require File.expand_path('../lib/cql_client/version', __FILE__)

Gem::Specification.new do |s|
  s.name = %q{cql_client}
  s.description = %q{A Ruby gem for accessing Cassandra using libcqlclient.}
  s.version = CqlClient::VERSION
  s.authors = ["Brent Theisen"]
  s.license = "Apache"
  s.email = %q{brent@bantamlabs.com}
  s.extensions = ["ext/cql_client/extconf.rb"]
  s.homepage = %q{http://github.com/brenttheisen/cql_client}
  s.rdoc_options = ["--charset=UTF-8"]
  s.summary = %q{A Ruby gem for accessing libcqlclient.}

  s.files = `git ls-files README.md LICENSE ext lib`.split
  s.test_files = `git ls-files spec examples`.split

  s.add_development_dependency 'rake-compiler'
  s.add_development_dependency 'rspec'
end
