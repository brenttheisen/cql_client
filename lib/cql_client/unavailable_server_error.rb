module CqlClient
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


