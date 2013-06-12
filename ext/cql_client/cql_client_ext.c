#include <ruby.h>
#include <cql_client_ext.h>

VALUE mCqlClient;
VALUE cClientError;
VALUE cServerError, cUnavailableServerError, cWriteTimeoutServerError, cReadTimeoutServerError,
  cAlreadyExistsServerError;
VALUE cArgumentError, cStandardError;

void Init_cql_client() {
	mCqlClient = rb_define_module("CqlClient");
  cClientError = rb_const_get(mCqlClient, rb_intern("ClientError"));
  cServerError = rb_const_get(mCqlClient, rb_intern("ServerError"));
  cUnavailableServerError = rb_const_get(mCqlClient, rb_intern("UnavailableServerError"));
  cWriteTimeoutServerError = rb_const_get(mCqlClient, rb_intern("WriteTimeoutServerError"));
  cReadTimeoutServerError = rb_const_get(mCqlClient, rb_intern("ReadTimeoutServerError"));
  cAlreadyExistsServerError = rb_const_get(mCqlClient, rb_intern("AlreadyExistsServerError"));

  cArgumentError = rb_const_get(rb_cObject, rb_intern("ArgumentError"));
  cStandardError = rb_const_get(rb_cObject, rb_intern("StandardError"));

  init_cql_client_connection();
  init_cql_result();
}

VALUE rb_raise_error(VALUE cError, char *message) {
  VALUE message_val = rb_str_new2(message);
  VALUE e = rb_exc_new3(cError, message_val);
  rb_exc_raise(e);

  return Qnil;
}
