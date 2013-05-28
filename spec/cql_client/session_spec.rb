require 'spec_helper'

describe CqlClient::Session do
  it 'should export C function' do
    CqlClient::Session.new.methods.should include(:bliggity)
    CqlClient::Session.new.bliggity.should == ''
  end
end
