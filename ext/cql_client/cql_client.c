#include <ruby.h>

VALUE cql_client_module = NULL;

void Init_cql_client(void) {
	cql_client_module = rb_define_module("CqlClient");
	
}