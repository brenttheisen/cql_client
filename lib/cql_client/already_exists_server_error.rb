module CqlClient
  class AlreadyExistsServerError < CqlClient::ServerError
    attr_accessor :keyspace, :table

    def initialize(msg)
      super(msg)
      @keyspace = nil
      @table = nil
    end
  end
end


