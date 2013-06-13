#include <cql_client_ext.h>

typedef struct {
  cql_result *result;
} result_wrapper;

extern VALUE mCqlClient;

static VALUE cRowsResult, cSetKeyspaceResult, cPreparedRowsResult, cSchemaChangeResult;

static VALUE symbol_created, symbol_updated, symbol_dropped;


static void rb_result_mark(void *wrapper) {
  result_wrapper *w = wrapper;
  if(w) {
    // Call rb_gc_mark on any Ruby alloc'd stuff
  }
}

static void rb_result_free(void *wrapper) {
  result_wrapper *w = wrapper;

  if(wrapper) {
    cql_result_destroy(w->result);

    // Call xfree on any Ruby alloc'd stuff
  }
}

VALUE rb_cql_result_to_obj(cql_result *result) {
  VALUE obj;

  switch(result->kind) {
  case CQL_RESULT_KIND_VOID:
    obj = Qtrue;
    break;
  case CQL_RESULT_KIND_ROWS:
    {
      result_wrapper *wrapper;
      obj = Data_Make_Struct(cRowsResult, result_wrapper, rb_result_mark, rb_result_free, wrapper);
      wrapper->result = result;
      rb_call_init(obj, 0, NULL);
    }
    break;
  case CQL_RESULT_KIND_SET_KEYSPACE:
    {
      VALUE values[1];
      values[0] = result->data ? rb_str_new2(result->data) : Qnil;
      obj = rb_class_new_instance(1, values, cSetKeyspaceResult);
    }
    break;
  case CQL_RESULT_KIND_PREPARED:
    // TODO Implement this
    // obj = Data_Make_Struct(cPreparedRowsResult, result_wrapper, rb_result_mark, rb_result_free, wrapper);
    break;
  case CQL_RESULT_KIND_SCHEMA_CHANGE:
    {
      cql_schema_change *schema_change = result->data;
      VALUE change = Qnil;
      if(schema_change->change) {
        if(strcmp(schema_change->change, "CREATED") == 0)
          change = symbol_created;
        else if(strcmp(schema_change->change, "UPDATED") == 0)
          change = symbol_updated;
        else if(strcmp(schema_change->change, "DROPPED") == 0)
          change = symbol_dropped;
      }

      VALUE keyspace = schema_change->keyspace ? rb_str_new2(schema_change->keyspace) : Qnil;
      VALUE table = schema_change->table ? rb_str_new2(schema_change->table) : Qnil;

      VALUE values[3];
      values[0] = change;
      values[1] = keyspace;
      values[2] = table;

      obj = rb_class_new_instance(3, values, cSchemaChangeResult);
    }
    break;
  default:
    obj = rb_raise_error(cStandardError, "Unknown result kind");
  }

  if(result->kind != CQL_RESULT_KIND_ROWS)
    cql_result_destroy(result);

  return obj;
}

void init_cql_result() {
  cRowsResult = rb_const_get(mCqlClient, rb_intern("RowsResult"));
  cSetKeyspaceResult = rb_const_get(mCqlClient, rb_intern("SetKeyspaceResult"));
  cPreparedRowsResult = rb_const_get(mCqlClient, rb_intern("PreparedRowsResult"));
  cSchemaChangeResult = rb_const_get(mCqlClient, rb_intern("SchemaChangeResult"));

  symbol_created = ID2SYM(rb_intern("created"));
  symbol_updated = ID2SYM(rb_intern("updated"));
  symbol_dropped = ID2SYM(rb_intern("dropped"));
}
