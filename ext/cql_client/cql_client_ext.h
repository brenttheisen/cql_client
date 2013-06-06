#ifndef CQL_CLIENT_EXT_H
#define CQL_CLIENT_EXT_H

#include <ruby.h>
#include <ruby/encoding.h>

#include <cql.h>
#include <result.h>

VALUE rb_raise_argument_error(char *message);

#endif // CQL_CLIENT_EXT_H
