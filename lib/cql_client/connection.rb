module CqlClient
  class Connection
    def initialize(hostname, port)
      @hostname = hostname
      @port = port

      initialize_ext

      connect @hostname, @port
    end
  end
end


