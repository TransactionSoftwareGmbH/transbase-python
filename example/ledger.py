from transbase import transbase
from ledger_utils import hashLedgerRecord, verifyAuditProof, verifyConsistencyProof

client = transbase.connect("//localhost:2024/sample", "tbadmin", "")
cursor = client.cursor()

cursor.execute("select * from ledger_hash()")
print(cursor.fetchall()[0])
# ['ca9c7838b42fd572f507587a234704115df1e84d02b69ea2196597e62de67e71', '5']

cursor.execute("select record_id from names where name = 'liam'")
print(cursor.fetchall()[0])
# ['5ae716a53bca41c7b249803a2d2ef919']

cursor.execute("select * from names where name = 'liam'")
liam = cursor.fetchall()[0]

hash = hashLedgerRecord(liam)
print(hash.hex())
# 9f1f297655f514f6cbed19b6b767a0a8925c80d453bd2ffa04e0cff20d7aef00

print(
    "audit proof",
    verifyAuditProof(
        cursor,
        bytes.fromhex(
            "ca9c7838b42fd572f507587a234704115df1e84d02b69ea2196597e62de67e71"
        ),
        5,
        bytes.fromhex("5ae716a53bca41c7b249803a2d2ef919"),
        hashLedgerRecord(liam),
    ),
)


print(
    "consistency proof",
    verifyConsistencyProof(
        cursor,
        bytes.fromhex(
            "140eaa105b724e7ed273ccb2562ba53f5429a93b74eb1fe1a5fda86ee5f92ca1"
        ),
        0,
        bytes.fromhex(
            "ca9c7838b42fd572f507587a234704115df1e84d02b69ea2196597e62de67e71"
        ),
        5,
    ),
)

cursor.close()
client.close()
