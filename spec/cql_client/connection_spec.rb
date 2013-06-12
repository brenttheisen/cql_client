require 'spec_helper'

describe CqlClient::Connection do
  let(:connection) { CqlClient::Connection.new 'localhost', '9042' }

  it 'should connect' do
    CqlClient::Connection.any_instance.stub(:connect)
    connection = CqlClient::Connection.new('localhost', '9042')
    CqlClient::Connection.any_instance.unstub(:connect)

    connection.send(:connect, 'localhost', '9042')
  end

  context 'when querying' do
    it 'should raise an exception with an unknown consistency level' do
      lambda { connection.query 'bogus query', :bliggity }.should raise_error(ArgumentError, 'Unknown consistency value')
    end

    it 'should query' do
      result = connection.query "create keyspace test_cql_client with replication = {'class': 'SimpleStrategy', 'replication_factor' : 1}", :any
    end
  end

  it 'should close' do
    connection.send(:close)
  end
end
