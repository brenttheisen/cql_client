require 'mkmf'

def asplode lib
  abort "-----\n#{lib} is missing.  please check your installation of mysql and try again.\n-----"
end

asplode('cql.h') if !have_header('cql.h')

dir_config('cql_client')
create_makefile('cql_client/cql_client')
