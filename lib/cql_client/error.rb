module CqlClient
  class ClientError < StandardError
  end

  class ServerError < StandardError
    attr_accessor :error_number

    def initialize(msg)
      super
      @error_number = error_number
    end
  end

  class AlreadyExistsServerError < CqlClient::ServerError
    attr_accessor :keyspace, :table

    def initialize(msg)
      super(msg)
      @keyspace = nil
      @table = nil
    end
  end

  class ReadTimeoutServerError < CqlClient::ServerError
    attr_accessor :consistency, :nodes_received, :nodes_required

    def initialize(msg)
      super(msg)
      @consistency = nil
      @nodes_received = nil
      @nodes_required = nil
    end
  end

  class WriteTimeoutServerError < CqlClient::ServerError
    attr_accessor :consistency, :nodes_received, :nodes_required

    def initialize(msg)
      super(msg)
      @consistency = nil
      @nodes_received = nil
      @nodes_required = nil
    end
  end

  class UnavailableServerError < CqlClient::ServerError
    attr_accessor :consistency, :required_nodes, :alive_nodes

    def initialize(msg)
      super(msg)
      @consistency = nil
      @required_nodes = nil
      @alive_nodes = nil
    end
  end
end

