from transbase import transbase

client = transbase.connect("//localhost:2024/sample", "tbadmin", "")
cursor = client.cursor()

cursor.execute("select * from cashbook;")
print(cursor.fetchall())

cursor.close()
client.close()
