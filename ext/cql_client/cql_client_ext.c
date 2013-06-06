#include <ruby.h>
#include <cql_client_ext.h>

VALUE mCqlClient;
VALUE cClientError;
VALUE cServerError, cUnavailableServerError, cWriteTimeoutServerError, cReadTimeoutServerError,
  cAlreadyExistsServerError;

void Init_cql_client() {
	mCqlClient = rb_define_module("CqlClient");
  cClientError = rb_const_get(mCqlClient, rb_intern("ClientError"));
  cServerError = rb_const_get(mCqlClient, rb_intern("ServerError"));
  cUnavailableServerError = rb_const_get(mCqlClient, rb_intern("UnavailableServerError"));
  cWriteTimeoutServerError = rb_const_get(mCqlClient, rb_intern("WriteTimeoutServerError"));
  cReadTimeoutServerError = rb_const_get(mCqlClient, rb_intern("ReadTimeoutServerError"));
  cAlreadyExistsServerError = rb_const_get(mCqlClient, rb_intern("AlreadyExistsServerError"));

  init_cql_client_connection();
}
