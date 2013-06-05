require 'mkmf'

def asplode lib
  abort "-----\n#{lib} is missing.  please check your installation of mysql and try again.\n-----"
end

asplode('cql.h') if !have_header('cql.h')

have_library('cqlclient', 'cql_connection_create')

create_makefile('cql_client/cql_client')
