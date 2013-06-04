module CqlClient
  class ServerError < StandardError
    attr_accessor :error_number

    def initialize(msg)
      super
      @error_number = error_number
    end
  end
end

