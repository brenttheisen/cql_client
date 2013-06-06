#include <cql_client_ext.h>

typedef struct {
  cql_result *result;
} result_wrapper;

extern VALUE mCqlClient;

VALUE cRowsResult, cSetKeyspaceResult, cPreparedRowsResult, cSchemaChangeResult;


static void rb_result_mark(void *wrapper) {
  result_wrapper *w = wrapper;
  if(w) {
    // Call rb_gc_mark on any Ruby alloc'd stuff
  }
}

static void rb_result_free(void *wrapper) {
  result_wrapper *w = wrapper;

  // TODO Close connection

  // Call xfree on any Ruby alloc'd stuff
}

VALUE rb_cql_result_to_obj(cql_result *result) {
  result_wrapper *wrapper;

  VALUE obj;
  switch(result->kind) {
  case CQL_RESULT_KIND_ROWS:
    obj = Data_Make_Struct(cRowsResult, result_wrapper, rb_result_mark, rb_result_free, wrapper);
    break;
  case CQL_RESULT_KIND_SET_KEYSPACE:
    obj = Data_Make_Struct(cSetKeyspaceResult, result_wrapper, rb_result_mark, rb_result_free, wrapper);
    break;
  case CQL_RESULT_KIND_PREPARED:
    obj = Data_Make_Struct(cPreparedRowsResult, result_wrapper, rb_result_mark, rb_result_free, wrapper);
    break;
  case CQL_RESULT_KIND_SCHEMA_CHANGE:
    obj = Data_Make_Struct(cSchemaChangeResult, result_wrapper, rb_result_mark, rb_result_free, wrapper);
    break;
  default:
    // TODO Raise client error
    return Qnil;
  }

  wrapper->result = result;
  rb_obj_call_init(obj, 0, NULL);

  return obj;
}

void init_cql_result() {
  cRowsResult = rb_const_get(mCqlClient, rb_intern("RowsResult"));
  cSetKeyspaceResult = rb_const_get(mCqlClient, rb_intern("SetKeyspaceResult"));
  cPreparedRowsResult = rb_const_get(mCqlClient, rb_intern("PreparedRowsResult"));
  cSchemaChangeResult = rb_const_get(mCqlClient, rb_intern("SchemaChangeResult"));
}
