#include <ruby.h>

VALUE rb_mCqlClient;


static VALUE session_create() {
  return rb_str_new2("blah!!!");
}

void Init_cql_client(void) {
	rb_mCqlClient = rb_define_module("CqlClient");
  VALUE rb_cSession = rb_define_class_under(rb_mCqlClient, "Session", rb_cObject);
  rb_define_method(rb_cSession, "bliggity", session_create, 0);
}
