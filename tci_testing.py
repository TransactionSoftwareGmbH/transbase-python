# cmult_testing.py
import ctypes as ct 
import sys

if __name__ == "__main__":
    # Load the shared library into c types.
    if sys.platform.startswith("win"):
        tci = ct.CDLL("./.transbase/lib/tci.dll")
    else:
        tci = ct.CDLL("./.transbase/lib/libtci.so")

TCIEnvironment = ct.POINTER(ct.c_void_p)
TCIError = ct.POINTER(ct.c_void_p)
TCIConnection = ct.POINTER(ct.c_void_p)
TCIStatement = ct.POINTER(ct.c_void_p)
TCIResultSet = ct.POINTER(ct.c_void_p)

env = TCIEnvironment()
connection = TCIConnection()
error = TCIError()
statement = TCIStatement()
resultset = TCIResultSet()

state = tci.TCIAllocEnvironmentW(ct.byref(env))
print(f"alloc environment: {state} {env[0]}")

state = tci.TCIAllocErrorW(env, ct.byref(error))
print(f"alloc error: {state}")

state = tci.TCIAllocConnectionW(env, error, ct.byref(connection))
print(f"alloc connection: {state}")

state = tci.TCIConnectW(connection, "//localhost:2024/sample")
print(f"connect: {state}")

state = tci.TCILoginW(connection, "tbadmin", "")
print(f"login: {state}")

state = tci.TCIAllocStatementW(connection, error, ct.byref(statement))
print(f"alloc statement {state}")

state = tci.TCIAllocResultSetW (statement , error, ct.byref(resultset))
print(f"alloc resultset {state}")

state = tci.TCIExecuteDirectW (resultset , "select * from cashbook" , 1 ,0)
print(f"execute query {state}")

state =tci.TCIFetchW(resultset, 1, 1, 0)
print(f"fetch {state}")

TCI_C_CHAR = 0x0100 | 0x1000 | 0x0a
for col in [1,2,3,4]:
    value = ct.create_string_buffer(1024)
    state = tci.TCIGetDataW(resultset, col, ct.byref(value), ct.sizeof(value), None, TCI_C_CHAR, None)
    print(f"get col {col} is {state}: {value.value}")

# free
if resultset:
    tci.TCIFreeResultSetW(resultset)
if  statement:
    tci.TCIFreeStatementW(statement)
if connection:
    tci.TCIFreeConnectionW(connection)
if  error:
    tci.TCIFreeErrorW(error)
if env:
    tci.TCIFreeEnvironmentW(env)