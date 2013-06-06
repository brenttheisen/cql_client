module CqlClient
  class Result
  end

  class RowsResult < Result
    include Enumerable
  end

  class SetKeyspaceResult < Result
  end

  class PreparedRowsResult < RowsResult
  end

  class SchemaChangeResult < Result
  end
end
