import hashlib

from transbase import transbase


def hashLedgerRecord(record: list[str]):
    hash = hashlib.sha256()
    hash.update(bytes(0))
    for value in record:
        if value is None:
            hash.update(bytes(0))
        else:
            hash.update(bytes(1))
            hash.update(value.encode("utf-8"))
    return hash.digest()


def verifyAuditProof(
    cursor: transbase.Cursor, ledgerHash, ledgerIdx, recordId, recordHash
):
    hashProof = recordHash

    cursor.type_cast = True
    cursor.execute(
        "select hash, first from ledger_audit_proof(?, ?) order by level asc",
        [ledgerIdx, recordId],
    )
    rows = cursor.fetchall()
    for [hash, first] in rows:
        if first:
            hashProof = hashLedgerNodes(hash, hashProof)
        else:
            hashProof = hashLedgerNodes(hashProof, hash)

    return equals(hashProof, ledgerHash)


def verifyConsistencyProof(
    cursor: transbase.Cursor, ledgerHashOld, ledgerIdxOld, ledgerHashNew, ledgerIdxNew
):
    hashOld = None
    hashNew = None
    cursor.type_cast = True
    cursor.execute(
        "select hash, first, old, new from ledger_consistency_proof(?, ?) order by new asc",
        [ledgerIdxOld, ledgerIdxNew],
    )

    rows = cursor.fetchall()
    for [hash, first, old, new] in rows:
        hashOld = calculateHash(old, first, hashOld, hash)
        hashNew = calculateHash(new, first, hashNew, hash)

    return equals(hashOld, ledgerHashOld) and equals(hashNew, ledgerHashNew)


def hashLedgerNodes(first, second):
    hash = hashlib.sha256()
    hash.update(bytes([1]))
    hash.update(first)
    hash.update(second)
    return hash.digest()


def calculateHash(index, first, currentHash, hash):
    if index is not None:
        if currentHash is None:
            return hash
        return (
            hashLedgerNodes(hash, currentHash)
            if first
            else hashLedgerNodes(currentHash, hash)
        )
    return currentHash


def equals(a, b):
    return a == b
