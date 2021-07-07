# https://www.python.org/dev/peps/pep-0249/
from transbase import tci

# GLOBALS
apilevel = "2.0"
threadsafety = 0
paramstyle = "qmark,named"


"""
These objects represent a database cursor, which is used to manage the context of a fetch operation
"""


class Cursor:
    """ "
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
    This read/write attribute specifies the number of rows to fetch at a time with .fetchmany(). It defaults to 1 meaning to fetch a single row at a time
    """
    arraysize = 1

    __statement = tci.TCIEnvironment()
    __resultset = tci.TCIResultSet()

    def __init__(self, connection, error):
        self.__state = tci.allocateStatement(connection, error, self.__statement)
        self.__state = tci.allocateResultSet(self.__statement, error, self.__resultset)

    def __del__(self):
        self.close()

    def execute(self, operation: str, params=None):
        """
        Prepare and execute a database operation (query or command).
        Parameters may be provided as sequence or mapping and will be bound to variables in the operation
        """
        # TODO params
        self.__state = tci.executeDirect(self.__resultset, operation, 1, 0)

    def executemany(self, operation: str, seq_of_parameters=[]):
        """
        Prepare a database operation (query or command) and then execute it against all parameter sequences or mappings found in the sequence seq_of_parameters.
        """
        # for loop, optimize later
        pass

    def fetchone(self):
        """
        Fetch the next row of a query result set, returning a single sequence, or None when no more data is available.
        """
        self.__state = tci.fetch(self.__resultset, 1, tci.TCI_FETCH_NEXT, 0)

    def fetchmany(self, size=None):
        """
        Fetch the next set of rows of a query result, returning a sequence of sequences (e.g. a list of tuples). An empty sequence is returned when no more rows are available.
        """
        pass

    def fetchall(self):
        """
        Fetch all (remaining) rows of a query result, returning them as a sequence of sequences (e.g. a list of tuples). Note that the cursor's arraysize attribute can affect the performance of this operation.
        """
        pass

    def setinputsizes(self, sizes):
        """
        This can be used before a call to .execute*() to predefine memory areas for the operation's parameters.
        """
        pass

    def setoutputsize(self, size, column=None):
        """
        Set a column buffer size for fetches of large columns (e.g. LONGs, BLOBs, etc.). The column is specified as an index into the result sequence. Not specifying the column will set the default size for all large columns in the cursor.
        """
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
        # get row size, column info
        pass

    def __getRow(self):
        # get row data
        pass

    # def callproc( procname [, parameters ] )


class Connection:
    __env = tci.TCIEnvironment()
    __con = tci.TCIConnection()
    __error = tci.TCIError()

    def __init__(self, url, user, password):
        tci.allocateEnvironment(self.__env)
        tci.allocateError(self.__env, self.__error)
        tci.allocateConnection(self.__env, self.__error, self.__con)
        self.__state = tci.connect(self.__con, url)
        self.__state = tci.login(self.__con, user, password)

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
        pass

    def rollback(self):
        """This method is optional since not all databases provide transaction support"""
        pass

    def cursor(self) -> Cursor:
        """Return a new Cursor Object using the connection."""
        return Cursor(self.__con, self.__error)

    def state(self) -> bool:
        return self.__state


# Constructor for creating a connection to the database
def connect(url: str, user: str, password: str) -> Connection:
    return Connection(url, user, password)
