require 'spec_helper'

describe CqlClient::SetKeyspaceResult do
  let(:connection) { CqlClient::Connection.new 'localhost', '9042' }

  before :each do
    connection.query 'drop keyspace test_cql_client', :any rescue nil
    connection.query "create keyspace test_cql_client with replication = {'class': 'SimpleStrategy', 'replication_factor' : 1}", :any
  end

  after :each do
    connection.query 'drop keyspace test_cql_client', :any
  end

  it 'should contain keyspace changed to' do
    result = connection.query 'use test_cql_client', :any
    result.should be_an_instance_of(CqlClient::SetKeyspaceResult)
    result.keyspace.should == 'test_cql_client'
  end
end

describe CqlClient::SchemaChangeResult do
  let(:connection) { CqlClient::Connection.new 'localhost', '9042' }

  it 'should handle created' do
    result = connection.query "create keyspace test_cql_client with replication = {'class': 'SimpleStrategy', 'replication_factor' : 1}", :any
    result.should be_an_instance_of(CqlClient::SchemaChangeResult)
    result.change.should == :created
    result.keyspace.should == 'test_cql_client'
    result.table.should == ''

    result = connection.query "create table test_cql_client.bliggity (blah int primary key)", :any
    result.should be_an_instance_of(CqlClient::SchemaChangeResult)
    result.change.should == :created
    result.keyspace.should == 'test_cql_client'
    result.table.should == 'bliggity'
  end
end
