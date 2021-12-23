import ctypes as ct
import sys
from transbase.error import DatabaseError, Error
import os.path


def find_tci_lib():
    lib_name = None
    if sys.platform.startswith("win"):
        lib_name = "tci.dll"
    elif sys.platform.startswith("darwin"):
        lib_name = "libtci.dylib"
    else:
        lib_name = "libtci.so"

    packageDirectory = os.path.dirname(os.path.abspath(__file__)) + os.path.sep
    search_locations = [
        (
            packageDirectory + "lib" + os.path.sep + lib_name
        ),  # absolute site-packages/transbase/lib/ (e.g. when intalled with install_tci)
        (
            packageDirectory + ".." + os.path.sep + "lib" + os.path.sep + lib_name
        ),  # packaged whithin wheel ../lib
        (
            "." + os.path.sep + "lib" + os.path.sep + lib_name
        ),  # ./lib for local development, or if download manually there
    ]

    for path in search_locations:
        if os.path.isfile(path):
            return path
    raise Error(
        "\033[91m"
        + f"Unable to find tci sdk ({lib_name}) searched in {search_locations}.\n Please run 'install_tci' command."
        + "\033[0m"
    )


# load tci dll
if __name__ == "transbase.tci":
    tci = ct.CDLL(find_tci_lib())

sizeof = ct.sizeof

TCIEnvironment = ct.POINTER(ct.c_void_p)
TCIError = ct.POINTER(ct.c_void_p)
TCIConnection = ct.POINTER(ct.c_void_p)
TCIStatement = ct.POINTER(ct.c_void_p)
TCIResultSet = ct.POINTER(ct.c_void_p)
TCIState = ct.c_int
TCIErrorCode = ct.c_int
attribute = ct.c_int

"""
ALLOCATIONS
"""
tci.TCIAllocEnvironmentW.argtypes = [ct.POINTER(TCIEnvironment)]
tci.TCIAllocEnvironmentW.restype = TCIState
allocateEnvironment = tci.TCIAllocEnvironmentW

tci.TCIAllocErrorW.argtypes = [TCIEnvironment, ct.POINTER(TCIError)]
tci.TCIAllocErrorW.restype = TCIState
allocateError = tci.TCIAllocErrorW

tci.TCIAllocConnectionW.argtypes = [TCIEnvironment, TCIError, ct.POINTER(TCIConnection)]
tci.TCIAllocConnectionW.restype = TCIState
allocateConnection = tci.TCIAllocConnectionW

tci.TCIAllocStatementW.argtypes = [TCIConnection, TCIError, ct.POINTER(TCIStatement)]
tci.TCIAllocStatementW.restype = TCIState
allocateStatement = tci.TCIAllocStatementW

tci.TCIAllocResultSetW.argtypes = [TCIStatement, TCIError, ct.POINTER(TCIResultSet)]
tci.TCIAllocResultSetW.restype = TCIState
allocateResultSet = tci.TCIAllocResultSetW

"""
FUNCTIONS
"""
tci.TCIConnectW.argtypes = [TCIConnection, ct.c_wchar_p]
tci.TCIConnectW.restype = TCIState
connect = tci.TCIConnectW

tci.TCILoginW.argtypes = [TCIConnection, ct.c_wchar_p, ct.c_wchar_p]
tci.TCILoginW.restype = TCIState
login = tci.TCILoginW


tci.TCIGetErrorW.argtypes = [
    TCIError,
    ct.c_int,
    ct.c_int,
    ct.c_wchar_p,
    ct.c_int,
    ct.POINTER(TCIErrorCode),
    ct.c_wchar_p,
]
tci.TCIGetErrorW.restype = TCIState
getError = tci.TCIGetErrorW


def handle_error(error):
    message = ct.create_unicode_buffer("", 1024)
    code = TCIErrorCode()
    sql_code = ct.create_unicode_buffer("", 1024)
    getError(
        error,
        1,
        1,
        message,
        sizeof(message),
        code,
        sql_code,
    )
    raise DatabaseError(code.value, message.value, sql_code.value)


tci.TCIExecuteDirectW.argtypes = [TCIResultSet, ct.c_wchar_p, ct.c_int, ct.c_int]
tci.TCIExecuteDirectW.restype = TCIState
executeDirect = tci.TCIExecuteDirectW

tci.TCIExecuteW.argtypes = [TCIResultSet, ct.c_int, ct.c_int]
tci.TCIExecuteW.restype = TCIState
execute = tci.TCIExecuteW

tci.TCIPrepareW.argtypes = [TCIStatement, ct.c_wchar_p]
tci.TCIPrepareW.restype = TCIState
prepare = tci.TCIPrepareW

tci.TCIFetchW.argtypes = [TCIResultSet, ct.c_int, ct.c_int, ct.c_int]
tci.TCIFetchW.restype = TCIState
fetch = tci.TCIFetchW


def get_tci_type_and_c_value(value):
    if type(value) == int:
        (c_value, tci_type, by_ref) = (ct.c_long(value), TCI_C_INT8, True)
    elif type(value) == float:
        (c_value, tci_type, by_ref) = (ct.c_double(value), TCI_C_DOUBLE, True)
    elif type(value) == bool:
        (c_value, tci_type, by_ref) = (ct.c_bool(value), TCI_C_INT1, True)
    elif type(value) == bytearray or type(value) == bytes:
        (c_value, tci_type, by_ref) = (
            ct.create_string_buffer(value),
            TCI_C_BYTE,
            False,
        )
    elif type(value) == str:
        (c_value, tci_type, by_ref) = (ct.c_wchar_p(value), TCI_C_WCHAR, False)
    else:
        (c_value, tci_type, by_ref) = (ct.c_wchar_p(str(value)), TCI_C_WCHAR, False)

    is_null = ct.c_int(-1) if value is None else None

    return (c_value, tci_type, by_ref, is_null)


def set_param(resultset, index, value):
    (c_value, tci_type, by_ref, is_null) = get_tci_type_and_c_value(value)
    return tci.TCISetDataW(
        resultset,
        index,
        c_value if not by_ref else ct.byref(c_value),
        sizeof(c_value),
        tci_type,
        is_null,
    )


def set_param_by_name(resultset, name, value):
    (c_value, tci_type, by_ref, is_null) = get_tci_type_and_c_value(value)
    return tci.TCISetDataByNameW(
        resultset,
        name,
        c_value if not by_ref else ct.byref(c_value),
        sizeof(c_value),
        tci_type,
        is_null,
    )


getData = tci.TCIGetDataW


def get_data_as_string(resultset, colNo=1):
    col = ct.c_int(colNo)
    byteSize = attribute(0)
    charLength = attribute(0)
    isNull = ct.c_bool()
    tci.TCIGetDataSizeW(resultset, col, TCI_C_WCHAR, ct.byref(byteSize), None)
    tci.TCIGetDataCharLengthW(resultset, col, ct.byref(charLength), None)
    buffer = ct.create_unicode_buffer(max(byteSize.value, charLength.value) + 1)
    getData(
        resultset,
        col,
        ct.byref(buffer),
        ct.sizeof(buffer),
        None,
        TCI_C_WCHAR,
        ct.byref(isNull),
    )
    return None if isNull else buffer.value


getResultSetAttribute = tci.TCIGetResultSetAttributeW


def resultset_attribute(resultset, attributekey, col=1):
    attr = attribute(0)
    tci.TCIGetResultSetAttributeW(
        resultset, attributekey, col, ct.byref(attr), sizeof(attr), None
    )
    return attr.value


def resultset_string_attribute(resultset, attributekey, col=1):
    attr = ct.create_unicode_buffer("", 127)
    tci.TCIGetResultSetAttributeW(
        resultset, attributekey, col, attr, sizeof(attr), None
    )
    return attr.value


"""
CLEANUP
"""
tci.TCIFreeResultSetW.argtypes = [TCIResultSet]
tci.TCIFreeResultSetW.restype = TCIState
freeResultSet = tci.TCIFreeResultSetW

tci.TCIFreeStatementW.argtypes = [TCIStatement]
tci.TCIFreeStatementW.restype = TCIState
freeStatement = tci.TCIFreeStatementW

tci.TCIFreeConnectionW.argtypes = [TCIConnection]
tci.TCIFreeConnectionW.restype = TCIState
freeConnection = tci.TCIFreeConnectionW

tci.TCIFreeErrorW.argtypes = [TCIError]
tci.TCIFreeErrorW.restype = TCIState
freeError = tci.TCIFreeErrorW

tci.TCIFreeEnvironmentW.argtypes = [TCIEnvironment]
tci.TCIFreeEnvironmentW.restype = TCIState
freeEnvironment = tci.TCIFreeEnvironmentW

"""
CONSTANTS
"""
TCI_SUCCESS = 0
TCI_NO_DATA_FOUND = 100

TCI_FETCH_NEXT = 1

TCI_TYPE = 0x0100
TCI_C_CHAR = TCI_TYPE | 0x1000 | 0x0A
TCI_C_WCHAR = TCI_C_CHAR | (0x2000)
TCI_C_INT1 = TCI_TYPE | 0x01
TCI_C_INT_4 = TCI_TYPE | 0x03
TCI_C_INT8 = TCI_TYPE | 0x0B
TCI_C_DOUBLE = TCI_TYPE | 0x06
TCI_C_BYTE = TCI_TYPE | 0x1000 | 0x0C

TCI_ATTR_COLUMN_COUNT = 7
TCI_ATTR_COLUMN_NAME = 10
TCI_ATTR_COLUMN_TYPE = 14
TCI_ATTR_ROWCOUNT = 40
TCI_ATTR_RECORDS_TOUCHED = 55

TCI_SQL_TYPES = {
    "BOOL": 16641,
    "TINYINT": 16642,
    "SMALLINT": 16643,
    "INTEGER": 16644,
    "NUMERIC": 16645,
    "FLOAT": 16646,
    "DOUBLE": 16647,
    "CHAR": 16648,
    "VARCHAR": 16649,
    "BINARY": 16650,
    "BIT": 16651,
    "BLOB": 16652,
    "BITSHORT": 16654,
    "BIGINT": 16655,
    "CLOB": 16656,
    "DATE": 17781,
    "DATE_YEAR": 17783,
    "DATE_YEAR_TO_MONTH": 17782,
    "DATE_YEAR_TO_DAY": 17781,
    "DATE_YEAR_TO_HOUR": 17780,
    "DATE_YEAR_TO_MINUTE": 17779,
    "DATE_YEAR_TO_SECOND": 17778,
    "DATE_YEAR_TO_MILLISECOND": 17777,
    "DATE_MONTH": 17766,
    "DATE_MONTH_TO_DAY": 17765,
    "DATE_MONTH_TO_HOUR": 17764,
    "DATE_MONTH_TO_MINUTE": 17763,
    "DATE_MONTH_TO_SECOND": 17762,
    "DATE_MONTH_TO_MILLISECOND": 17761,
    "DATE_DAY": 17749,
    "DATE_DAY_TO_HOUR": 17748,
    "DATE_DAY_TO_MINUTE": 17747,
    "DATE_DAY_TO_SECOND": 17746,
    "DATE_DAY_TO_MILLISECOND": 17745,
    "DATE_HOUR": 17732,
    "DATE_HOUR_TO_MINUTE": 17731,
    "DATE_HOUR_TO_SECOND": 17730,
    "DATE_HOUR_TO_MILLISECOND": 17729,
    "DATE_MINUTE": 17715,
    "DATE_MINUTE_TO_SECOND": 17714,
    "DATE_MINUTE_TO_MILLISECOND": 17713,
    "DATE_SECOND": 17698,
    "DATE_SECOND_TO_MILLISECOND": 17697,
    "DATE_MILLISECOND": 17681,
    "TIME": 17730,
    "TIMESTAMP": 17777,
    "INTERVAL": 18688,
    "INTERVAL_YEAR": 18807,
    "INTERVAL_YEAR_TO_MONTH": 18806,
    "INTERVAL_MONTH": 18790,
    "INTERVAL_DAY": 18773,
    "INTERVAL_DAY_TO_HOUR": 18772,
    "INTERVAL_DAY_TO_MINUTE": 18771,
    "INTERVAL_DAY_TO_SECOND": 18770,
    "INTERVAL_DAY_TO_MILLISECOND": 18769,
    "INTERVAL_HOUR": 18756,
    "INTERVAL_HOUR_TO_MINUTE": 18755,
    "INTERVAL_HOUR_TO_SECOND": 18754,
    "INTERVAL_HOUR_TO_MILLISECOND": 18753,
    "INTERVAL_MINUTE": 18739,
    "INTERVAL_MINUTE_TO_SECOND": 18738,
    "INTERVAL_MINUTE_TO_MILLISECOND": 18737,
    "INTERVAL_SECOND": 18722,
    "INTERVAL_SECOND_TO_MILLISECOND": 18721,
    "INTERVAL_MILLISECOND": 18705,
}
TCI_SQL_TYPES_INVERTED = {v: k for k, v in TCI_SQL_TYPES.items()}
