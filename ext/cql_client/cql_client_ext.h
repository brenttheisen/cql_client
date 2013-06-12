#ifndef CQL_CLIENT_EXT_H
#define CQL_CLIENT_EXT_H

#include <ruby.h>
#include <ruby/encoding.h>

#include <cql.h>
#include <connection.h>
#include <result.h>

extern VALUE cArgumentError, cStandardError;

#define rb_raise_standard_error(msg) rb_raise_error(cStandardError, msg)
#define rb_raise_argument_error(msg) rb_raise_error(cArgumentError, msg)
VALUE rb_raise_error(VALUE cError, char *message);

#endif // CQL_CLIENT_EXT_H
