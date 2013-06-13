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

  it 'should handle updated' do
    result = connection.query("alter keyspace test_cql_client with durable_writes = false", :any)
    result.should be_an_instance_of(CqlClient::SchemaChangeResult)
    result.change.should == :updated
    result.keyspace.should == 'test_cql_client'
    result.table.should == ''

    result = connection.query('alter table test_cql_client.bliggity add whatever varchar', :any)
    result.should be_an_instance_of(CqlClient::SchemaChangeResult)
    result.change.should == :updated
    result.keyspace.should == 'test_cql_client'
    result.table.should == 'bliggity'
  end

  it 'should handle dropped' do
    result = connection.query('drop table test_cql_client.bliggity', :any)
    result.should be_an_instance_of(CqlClient::SchemaChangeResult)
    result.change.should == :dropped
    result.keyspace.should == 'test_cql_client'
    result.table.should == 'bliggity'

    result = connection.query("drop keyspace test_cql_client", :any)
    result.should be_an_instance_of(CqlClient::SchemaChangeResult)
    result.change.should == :dropped
    result.keyspace.should == 'test_cql_client'
    result.table.should == ''
  end
end

describe CqlClient::RowsResult do
  let(:connection) { CqlClient::Connection.new 'localhost', '9042' }

  before :all do
    connection.query 'drop keyspace test_cql_client', :any rescue nil
    connection.query "create keyspace test_cql_client with replication = {'class': 'SimpleStrategy', 'replication_factor' : 1}", :any
    connection.query(<<-EOC, :any)
      create table test_cql_client.test_table (
        id int primary key,
        whatever varchar
      )
    EOC
    connection.query("insert into test_cql_client.test_table (id, whatever) values (1, 'First')", :any)
    connection.query("insert into test_cql_client.test_table (id, whatever) values (2, 'Second')", :any)
  end

  after :all do
    connection.query 'drop keyspace test_cql_client', :any
  end

  it 'should count' do
    result = connection.query('select * from test_cql_client.test_table', :one)
    result.should be_an_instance_of(CqlClient::RowsResult)
    result.count.should == 2
  end

end
