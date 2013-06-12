module CqlClient
  class Result
  end

  class RowsResult < Result
    include Enumerable
  end

  class SetKeyspaceResult < Result
    attr_reader :keyspace

    def initialize(keyspace)
      @keyspace = keyspace
    end
  end

  class PreparedRowsResult < RowsResult
  end

  class SchemaChangeResult < Result
    attr_reader :change, :keyspace, :table

    def initialize(change, keyspace, table)
      @change = change
      @keyspace = keyspace
      @table = table
    end
  end
end
