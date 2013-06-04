#include <ruby.h>
#include <cql.h>


VALUE cConnection;
static ID intern_error_number_eql, intern_consistency_eql, intern_required_nodes_eql, intern_alive_nodes_eql,
  intern_nodes_received_eql, intern_nodes_required_eql, intern_keyspace_eql, intern_table_eql;

void init_cql_client_connection() {
  cConnection = rb_define_class_under(mCqlClient, "Connection", rb_cObject);
  rb_define_private_method(cConnection, "initialize_ext", initialize_ext, 0);
  rb_define_private_method(cConnection, "connect", rb_connect, 2;

  cServerError = rb_define_class_under(mCqlClient, "ServerError", rb_cObject);

  intern_error_number_eql = rb_intern("error_number=");
  intern_consistency_eql = rb_intern("consistency=");
  intern_required_nodes_eql = rb_intern("required_nodes=");
  intern_alive_nodes_eql = rb_intern("alive_nodes=");
  intern_nodes_received_eql = rb_intern("nodes_received=");
  intern_nodes_required_eql = rb_intern("nodes_required=");
  intern_keyspace_eql = rb_intern("keyspace=");
  intern_table_eql = rb_intern("table=");
}

static VALUE initialize_ext(VALUE self) {
  // TODO Is this needed?
}

static VALUE connect(VALUE self, VALUE host, VALUE port) {
  cql_host host_args;
  host_args = NIL_P(host) ? "localhost" : StringValuePtr(host);
  host_args = NIL_P(port) ? 9042 : StringValuePtr(port);

  void *result;
  int rc = cql_connection_create(&host_args, &result);
  if(process_common_results(rc, result))
    return Qnil;

  // TODO Do something with the cql_connection struct

  return Qnil;
}

int process_common_results(int rc, void *result) {
  switch(rc) {
  case CQL_RESULT_CLIENT_ERROR:
    {
      cql_client_error error = (cql_client_error*) result;

      VALUE msg = rb_str_new2(error->message);
      VALUE e = rb_exc_new3(cClientError, msg);
      rb_exc_raise(e);

      cql_client_error_destroy(error);
    }
    return 1;
  case CQL_RESULT_SERVER_ERROR:
    {
      cql_server_error error = (cql_server_error*) result;

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
          VALUE keyspace = error->keyspace ? rb_str_new2(error->keyspace) : Qnil;
          VALUE table = error->table ? rb_str_new2(error->table) : Qnil;
          rb_funcall(e, intern_keyspace_eql, 1, already_exists->keyspace);
          rb_funcall(e, intern_table_eql, 1, already_exists->table);
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

