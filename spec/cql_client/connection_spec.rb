require 'spec_helper'

describe CqlClient::Connection do
  let(:connection) { CqlClient::Connection.new 'localhost', '9042' }

  it 'should connect' do
    CqlClient::Connection.any_instance.stub(:connect)
    connection = CqlClient::Connection.new('localhost', '9042')
    CqlClient::Connection.any_instance.unstub(:connect)

    connection.send(:connect, 'localhost', '9042')
  end

  it 'should query' do
    result = connection.query 'create keyspace test_cql_client', :any
  end

  it 'should close' do
    connection.send(:close)
  end
end
