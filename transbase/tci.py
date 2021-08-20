import ctypes as ct
import sys

if __name__ == "transbase.tci":
    # Load the shared library into c types.
    if sys.platform.startswith("win"):
        tci = ct.CDLL("./lib/tci.dll")
    else:
        tci = ct.CDLL("./lib/libtci.so")

sizeof = ct.sizeof

TCIEnvironment = ct.POINTER(ct.c_void_p)
TCIError = ct.POINTER(ct.c_void_p)
TCIConnection = ct.POINTER(ct.c_void_p)
TCIStatement = ct.POINTER(ct.c_void_p)
TCIResultSet = ct.POINTER(ct.c_void_p)
TCIState = ct.c_int
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

tci.TCIExecuteDirectW.argtypes = [TCIResultSet, ct.c_wchar_p, ct.c_int, ct.c_int]
tci.TCIExecuteDirectW.restype = TCIState
executeDirect = tci.TCIExecuteDirectW

tci.TCIFetchW.argtypes = [TCIResultSet, ct.c_int, ct.c_int, ct.c_int]
tci.TCIFetchW.restype = TCIState
fetch = tci.TCIFetchW

# tci.TCIGetDataW.argtypes = [
#     TCIResultSet,
#     ct.c_int,
#     ct.c_wchar_p,
#     ct.c_int,
#     ct.c_void_p,
#     ct.c_int,
#     ct.POINTER(ct.c_char),
# ]
# tci.TCIGetDataW.restype = TCIState
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

TCI_FETCH_NEXT = 1

TCI_C_CHAR = 0x0100 | 0x1000 | 0x0A
TCI_C_WCHAR = TCI_C_CHAR | (0x2000)

TCI_ATTR_COLUMN_COUNT = 7
TCI_ATTR_COLUMN_NAME = 10
TCI_ATTR_COLUMN_TYPE = 14
TCI_ATTR_ROWCOUNT = 40
TCI_ATTR_RECORDS_TOUCHED = 55
