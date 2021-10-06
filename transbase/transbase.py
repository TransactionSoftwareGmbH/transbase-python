# https://www.python.org/dev/peps/pep-0249/
from collections import Mapping
from transbase import tci

# GLOBALS
apilevel = "2.0"
threadsafety = 0
paramstyle = "qmark,named"


class Cursor:
    """
    These objects represent a database cursor,
    which is used to manage the context of a fetch operation
    """

    """
    This read-only attribute is a sequence of 7-item sequences.
    Each of these sequences contains information describing one result column:
        - name
        - type_code
    (- display_size
        - internal_size
        - precision
        - scale
        - null_ok )
    """

    description = None
    """
    This read-only attribute specifies the number of rows that the last .execute*() produced
    """
    rowcount = 0
    """
    This read/write attribute specifies the number of rows to fetch at a time with .fetchmany().
    It defaults to 1 meaning to fetch a single row at a time
    """
    arraysize = 1

    __statement = tci.TCIEnvironment()
    __resultset = tci.TCIResultSet()

    def __init__(self, connection, error):
        self.__error = error
        self.__call(tci.allocateStatement(connection, error, self.__statement))
        self.__call(tci.allocateResultSet(self.__statement, error, self.__resultset))

    def __del__(self):
        self.close()

    def execute(self, operation: str, params=None):
        """
        Prepare and execute a database operation (query or command).
        Parameters may be provided as sequence or mapping and will be bound to variables in the operation
        """
        # execute query direct if no parameters are given
        if params is None:
            self.__call(tci.executeDirect(self.__resultset, operation, 1, 0))

        # prepare statement and set positional or named parameters before execution
        else:
            self.__call(tci.prepare(self.__statement, operation))
            if isinstance(params, Mapping):
                for name, value in params.items():
                    self.__call(tci.set_param_by_name(self.__resultset, name, value))
            else:
                for idx, param in enumerate(params, start=1):
                    self.__call(tci.set_param(self.__resultset, idx, param))

            self.__call(tci.execute(self.__resultset, 1, 0))

        self.__setResultSetMeta()
        self.__setRowCount()

    def executemany(self, operation: str, seq_of_parameters=[]):
        """
        Prepare a database operation (query or command) and then execute it against all parameter sequences
        or mappings found in the sequence seq_of_parameters.
        """
        # for loop, optimize later
        # TODO
        pass

    def fetchone(self):
        """
        Fetch the next row of a query result set, returning a single sequence, or None when no more data is available.
        """
        self.__state = tci.fetch(self.__resultset, 1, tci.TCI_FETCH_NEXT, 0)
        if self.__state == tci.TCI_SUCCESS:
            return self.__getRow()
        elif self.__state != tci.TCI_NO_DATA_FOUND:
            return self.__call(self.__state)
        else:
            return None

    def fetchmany(self, size=None):
        """
        Fetch the next set of rows of a query result, returning a sequence of sequences (e.g. a list of tuples).
        An empty sequence is returned when no more rows are available.
        Set size to -1 to fetch all remaining, see fetchall.
        """
        result = []
        count = size if size is not None else self.arraysize
        i = 0
        while count < 0 or i < count:
            i += 1
            # can be optimized later with tci fetch many...
            self.__state = tci.fetch(self.__resultset, 1, tci.TCI_FETCH_NEXT, 0)
            if self.__state == tci.TCI_SUCCESS:
                result.append(self.__getRow())
            elif self.__state != tci.TCI_NO_DATA_FOUND:
                self.__call(self.__state)
            else:
                break

        return result

    def fetchall(self):
        """
        Fetch all (remaining) rows of a query result, returning them as a sequence of sequences (e.g. a list of tuples).
        Note that the cursor's arraysize attribute can affect the performance of this operation.
        """
        return self.fetchmany(-1)

    def setinputsizes(self, sizes):
        """
        This can be used before a call to .execute*() to predefine memory areas for the operation's parameters.
        """
        # TODO (optional api)
        pass

    def setoutputsize(self, size, column=None):
        """
        Set a column buffer size for fetches of large columns (e.g. LONGs, BLOBs, etc.).
        The column is specified as an index into the result sequence. Not specifying the column will set the default size for all large columns in the cursor.
        """
        # TODO (optional api)
        pass

    def close(self):
        """
        Close the cursor now (rather than whenever __del__ is called)
        """
        if self.__resultset:
            tci.freeResultSet(self.__resultset)
            self.__resultset = None
        if self.__statement:
            tci.freeStatement(self.__statement)
            self.__statement = None
        self.__state = None

    def state(self):
        return self.__state

    def __setResultSetMeta(self):
        no_cols = tci.resultset_attribute(self.__resultset, tci.TCI_ATTR_COLUMN_COUNT)
        meta = []
        for col in range(1, no_cols + 1):
            colName = tci.resultset_string_attribute(
                self.__resultset, tci.TCI_ATTR_COLUMN_NAME, col
            )
            colType = tci.resultset_attribute(
                self.__resultset, tci.TCI_ATTR_COLUMN_TYPE, col
            )
            meta.append([colName, colType])

        self.description = meta

    def __setRowCount(self):
        self.rowcount = tci.resultset_attribute(
            self.__resultset,
            tci.TCI_ATTR_ROWCOUNT,
        )

    def __getRow(self):
        row = []
        for idx, _ in enumerate(self.description, start=1):
            row.append(tci.get_data_as_string(self.__resultset, idx))
        return row

    def __call(self, state):
        self.__state = state
        if self.__state != tci.TCI_SUCCESS:
            tci.handle_error(self.__error)

    # def callproc( procname [, parameters ] )


class Connection:
    __env = tci.TCIEnvironment()
    __con = tci.TCIConnection()
    __error = tci.TCIError()

    def __init__(self, url, user, password):
        tci.allocateEnvironment(self.__env)
        tci.allocateError(self.__env, self.__error)
        tci.allocateConnection(self.__env, self.__error, self.__con)
        self.__call(tci.connect(self.__con, url))
        self.__call(tci.login(self.__con, user, password))

    def __del__(self):
        self.close()

    def close(self):
        """
        Close the connection now (rather than whenever .__del__() is called).
        """
        if self.__con:
            tci.freeConnection(self.__con)
            self.__con = None
        if self.__error:
            tci.freeError(self.__error)
            self.__error = None
        if self.__env:
            tci.freeEnvironment(self.__env)
            self.__error = None
        self.__state = None

    def commit(self):
        """Commit any pending transaction to the database."""
        # TODO (optional api)
        pass

    def rollback(self):
        """This method is optional since not all databases provide transaction support"""
        # TODO (optional api)
        pass

    def cursor(self) -> Cursor:
        """Return a new Cursor Object using the connection."""
        return Cursor(self.__con, self.__error)

    def state(self) -> bool:
        return self.__state

    def __call(self, state):
        self.__state = state
        if self.__state != tci.TCI_SUCCESS:
            tci.handle_error(self.__error)


# Constructor for creating a connection to the database
def connect(url: str, user: str, password: str) -> Connection:
    return Connection(url, user, password)
