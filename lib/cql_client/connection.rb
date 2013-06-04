module CqlClient
  class Connection
    def initialize(host)
      @host = host

      initialize_ext

      connect @host
    end
  end
end


