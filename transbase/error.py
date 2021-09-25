class Error(Exception):
    """
    Exception that is the base class of all other error exceptions. You can use this to catch all errors with one single except statement. Warnings are not considered errors and thus should not use this class as base. It must be a subclass of the Python StandardError (defined in the module exceptions).
    """

    pass


class DatabaseError(Error):
    """
    Exception raised for errors that are related to the database.Subclass of Error.
    """

    pass


class InterfaceError(Error):
    """
    Exception raised for errors that are related to the database interface rather than the database itself. Subclass of Error."""

    pass
