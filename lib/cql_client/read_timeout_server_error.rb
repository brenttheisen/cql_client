module CqlClient
  class ReadTimeoutServerError < CqlClient::ServerError
    attr_accessor :consistency, :nodes_received, :nodes_required

    def initialize(msg)
      super(msg)
      @consistency = nil
      @nodes_received = nil
      @nodes_required = nil
    end
  end
end


