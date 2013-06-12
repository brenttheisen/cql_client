#include <cql_client_ext.h>

#define GET_WRAPPER(self) \
  connection_wrapper *wrapper; \
  Data_Get_Struct(self, connection_wrapper, wrapper)

typedef struct {
  cql_connection *connection;
  int initialized;
} connection_wrapper;

extern VALUE mCqlClient, cClientError, cServerError, cUnavailableServerError, cWriteTimeoutServerError,
  cReadTimeoutServerError, cAlreadyExistsServerError;

static ID intern_error_number_eql, intern_consistency_eql, intern_required_nodes_eql, intern_alive_nodes_eql,
  intern_nodes_received_eql, intern_nodes_required_eql, intern_keyspace_eql, intern_table_eql;
static ID intern_any, intern_one, intern_two, intern_three, intern_quorum, intern_all, intern_local_quorum,
  intern_each_quorum;

VALUE cConnection;

static void rb_connection_mark(void *wrapper) {
  connection_wrapper *w = wrapper;
  if(w) {
    // Call rb_gc_mark on any Ruby alloc'd stuff
  }
}

static void rb_connection_free(void *wrapper) {
  connection_wrapper *w = wrapper;

  // TODO Close connection

  // Call xfree on any Ruby alloc'd stuff
}

static VALUE allocate(VALUE klass) {
  connection_wrapper *wrapper;
  VALUE obj = Data_Make_Struct(klass, connection_wrapper, rb_connection_mark,
    rb_connection_free, wrapper);
  wrapper->initialized = 0;
  wrapper->connection = xmalloc(sizeof(cql_connection));

  return obj;
}

static VALUE initialize_ext(VALUE self) {
  // TODO Is this needed?
  GET_WRAPPER(self);

  wrapper->initialized = 1;
}

static VALUE rb_connect(VALUE self, VALUE host, VALUE port) {
  cql_host host_args;
  host_args.hostname = NIL_P(host) ? "localhost" : StringValuePtr(host);
  host_args.port = NIL_P(port) ? "9042" : StringValuePtr(port);

  void *result;
  int rc = cql_connection_create(&host_args, &result);
  if(handle_error_results(rc, result))
    return Qnil;

  GET_WRAPPER(self);
  wrapper->connection = result;

  return Qnil;
}

static VALUE rb_close(VALUE self) {
  GET_WRAPPER(self);

  cql_connection_destroy(wrapper->connection);

  return Qnil;
}

static VALUE rb_query(VALUE self, VALUE query, VALUE consistency) {
  GET_WRAPPER(self);

  Check_Type(query, T_STRING);
  char *query_val = StringValueCStr(query);

  Check_Type(consistency, T_SYMBOL);
  unsigned short consistency_val;
  ID consistency_id = SYM2ID(consistency);
  if(consistency_id == intern_any)
    consistency_val = CQL_CONSISTENCY_ANY;
  else if(consistency_id == intern_one)
    consistency_val = CQL_CONSISTENCY_ONE;
  else if(consistency_id == intern_two)
    consistency_val = CQL_CONSISTENCY_TWO;
  else if(consistency_id == intern_three)
    consistency_val = CQL_CONSISTENCY_THREE;
  else if(consistency_id == intern_quorum)
    consistency_val = CQL_CONSISTENCY_QUORUM;
  else if(consistency_id == intern_all)
    consistency_val = CQL_CONSISTENCY_ALL;
  else if(consistency_id == intern_local_quorum)
    consistency_val = CQL_CONSISTENCY_LOCAL_QUORUM;
  else if(consistency_id == intern_each_quorum)
    consistency_val = CQL_CONSISTENCY_EACH_QUORUM;
  else
    return rb_raise_argument_error("Unknown consistency value");

  void *result;
  int rc = cql_connection_query(wrapper->connection, query_val, consistency_val, &result);
  if(handle_error_results(rc, result))
    return Qnil;

  return handle_result(result);
}

int handle_result(cql_result *result) {
  switch(result->kind) {
  case CQL_RESULT_KIND_VOID:
    return Qtrue;
  case CQL_RESULT_KIND_ROWS:
    // TODO Implement this
    return Qnil;
  case CQL_RESULT_KIND_SET_KEYSPACE:
    // TODO Implement this
    return Qnil;
  case CQL_RESULT_KIND_PREPARED:
    // TODO Implement this
    return Qnil;
  case CQL_RESULT_KIND_SCHEMA_CHANGE:
    // TODO Implement this
    return Qnil;
  }
}

int handle_error_results(int rc, void *result) {
  switch(rc) {
  case CQL_RESULT_CLIENT_ERROR:
    {
      cql_client_error *error = (cql_client_error*) result;

      VALUE msg = rb_str_new2(error->message);
      VALUE e = rb_exc_new3(cClientError, msg);
      rb_exc_raise(e);

      cql_client_error_destroy(error);
    }
    return 1;
  case CQL_RESULT_SERVER_ERROR:
    {
      cql_server_error *error = (cql_server_error*) result;
      VALUE msg = error->message ? rb_str_new2(error->message) : Qnil;

      VALUE e;
      switch(error->code) {
      case CQL_ERROR_UNAVAILABLE:
        {
          cql_unavailable *unavailable = (cql_unavailable*) error->additional;

          e = rb_exc_new3(cUnavailableServerError, msg);
          rb_funcall(e, intern_consistency_eql, 1, UINT2NUM(unavailable->consistency));
          rb_funcall(e, intern_required_nodes_eql, 1, ULONG2NUM(unavailable->required_nodes));
          rb_funcall(e, intern_alive_nodes_eql, 1, ULONG2NUM(unavailable->alive_nodes));
        }
        break;
      case CQL_ERROR_WRITE_TIMEOUT:
        {
          cql_write_timeout *write_timeout = (cql_write_timeout*) error->additional;

          e = rb_exc_new3(cWriteTimeoutServerError, msg);
          rb_funcall(e, intern_consistency_eql, 1, UINT2NUM(write_timeout->consistency));
          rb_funcall(e, intern_nodes_received_eql, 1, ULONG2NUM(write_timeout->nodes_received));
          rb_funcall(e, intern_nodes_required_eql, 1, ULONG2NUM(write_timeout->nodes_required));
        }
        break;
      case CQL_ERROR_READ_TIMEOUT:
        {
          cql_read_timeout *read_timeout = (cql_read_timeout*) error->additional;

          e = rb_exc_new3(cReadTimeoutServerError, msg);
          rb_funcall(e, intern_consistency_eql, 1, UINT2NUM(read_timeout->consistency));
          rb_funcall(e, intern_nodes_received_eql, 1, ULONG2NUM(read_timeout->nodes_received));
          rb_funcall(e, intern_nodes_required_eql, 1, ULONG2NUM(read_timeout->nodes_required));
        }
        break;
      case CQL_ERROR_ALREADY_EXISTS:
        {
          cql_already_exists *already_exists = (cql_already_exists*) error->additional;

          e = rb_exc_new3(cAlreadyExistsServerError, msg);
          VALUE keyspace = already_exists->keyspace ? rb_str_new2(already_exists->keyspace) : Qnil;
          VALUE table = already_exists->table ? rb_str_new2(already_exists->table) : Qnil;
          rb_funcall(e, intern_keyspace_eql, 1, keyspace);
          rb_funcall(e, intern_table_eql, 1, table);
        }
        break;
      default:
        e = rb_exc_new3(cServerError, msg);
        break;
      }

      rb_funcall(e, intern_error_number_eql, 1, ULONG2NUM(error->code));
      rb_exc_raise(e);

      cql_server_error_destroy(error);
    }
    return 1;
  }

  return 0;
}

void init_cql_client_connection() {
  cConnection = rb_define_class_under(mCqlClient, "Connection", rb_cObject);

  rb_define_alloc_func(cConnection, allocate);

  rb_define_private_method(cConnection, "initialize_ext", initialize_ext, 0);
  rb_define_private_method(cConnection, "connect", rb_connect, 2);
  rb_define_private_method(cConnection, "close", rb_close, 0);
  rb_define_method(cConnection, "query", rb_query, 2);

  intern_error_number_eql = rb_intern("error_number=");
  intern_consistency_eql = rb_intern("consistency=");
  intern_required_nodes_eql = rb_intern("required_nodes=");
  intern_alive_nodes_eql = rb_intern("alive_nodes=");
  intern_nodes_received_eql = rb_intern("nodes_received=");
  intern_nodes_required_eql = rb_intern("nodes_required=");
  intern_keyspace_eql = rb_intern("keyspace=");
  intern_table_eql = rb_intern("table=");

  intern_any = rb_intern("any");
  intern_one = rb_intern("one");
  intern_two = rb_intern("two");
  intern_three = rb_intern("three");
  intern_quorum = rb_intern("quorum");
  intern_all = rb_intern("all");
  intern_local_quorum = rb_intern("local_quorum");
  intern_each_quorum = rb_intern("each_quorum");
}
