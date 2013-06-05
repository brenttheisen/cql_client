require 'spec_helper'

describe CqlClient::Connection do
  it 'should connect' do
    CqlClient::Connection.any_instance.stub(:connect)
    connection = CqlClient::Connection.new('localhost', '9042')
    CqlClient::Connection.any_instance.unstub(:connect)

    connection.send(:connect, 'localhost', '9042')
  end
end
