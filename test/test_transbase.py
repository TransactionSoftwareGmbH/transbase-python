import os
from transbase.error import DatabaseError
import unittest
from transbase import transbase
import uuid

db_url = os.getenv("DB_URL", "//localhost:2024/sample")
db_user = os.getenv("DB_USER", "tbadmin")
db_password = os.getenv("DB_PASSWORD", "")
db = (db_url, db_user, db_password)

TABLE = "cashbook_py_" + str(uuid.uuid4())[0 - 7]

SELECT_ALL = f"select * from {TABLE}"


class TestTransbase(unittest.TestCase):

    client = None

    @classmethod
    def setUpClass(cls):
        client = transbase.connect(*db)
        cursor = client.cursor()
        cursor.execute(
            f"""create table {TABLE}
       (
         nr 	integer not null primary key auto_increment,
         date 	timestamp not null default currentdate,
         amount 	numeric(10,2) not null,
         comment varchar(*),
       );"""
        )
        cursor.execute(
            f"insert into {TABLE} values (default, default, 100, 'Withdrawal');"
        )
        cursor.execute(
            f"insert into {TABLE} values (default, currentdate, -9.50, 'Lunch🚀');"
        )
        cursor.execute(f"insert into {TABLE} (amount, comment) values (-5.5, 'Drink');")
        cursor.execute(f"insert into {TABLE} values (default, '2021-02-20', 0, '');")
        cursor.execute(f"insert into {TABLE} (amount, comment) values (-2.5, null);")
        cursor.close()
        client.close()

    @classmethod
    def tearDownClass(cls):
        try:
            client = transbase.connect(*db)
            cursor = client.cursor()
            cursor.execute(f"drop table {TABLE};")
            cursor.close()
            client.close()
        except Exception as e:
            print(e)

    def setUp(self):
        self.client = transbase.connect(*db)

    def tearDown(self):
        if self.client:
            self.client.close()

    def test_connect_error(self):
        try:
            transbase.connect("//localhost:2024/what", "tbadmin", "")
        except Exception as e:
            self.assertIsInstance(e, DatabaseError)

    def test_connect_success(self):
        client = transbase.connect(*db)
        self.assertIsNotNone(client)
        self.assertIsInstance(client, transbase.Connection)
        self.assertEqual(client.state(), 0)

    def test_close_connection(self):
        client = transbase.connect(*db)
        client.close()
        self.assertEqual(client.state(), None)

    def test_execute_query(self):
        cursor = self.client.cursor()
        self.assertIsInstance(cursor, transbase.Cursor)
        self.assertIsNotNone(cursor)
        cursor.execute(SELECT_ALL)
        row = cursor.fetchone()
        self.assertEqual(cursor.state(), 0)
        self.assertIsNotNone(row)
        cursor.close()

    def test_result_set_meta(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL)
        self.assertEqual(len(cursor.description), 4)
        self.assertEqual(
            cursor.description,
            [["nr", 16644], ["date", 17777], ["amount", 16645], ["comment", 16649]],
        )
        cursor.close()

    def test_fetch_one_row(self):
        cursor = self.client.cursor()
        cursor.execute(f"select nr, amount, comment from {TABLE}")
        row = cursor.fetchone()
        self.assertIsNotNone(row)
        self.assertEqual(row, ["1", "100.00", "Withdrawal"])
        cursor.close()

    def test_fetch_one_return_none_if_no_data(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL + " where nr < 0")
        row = cursor.fetchone()
        self.assertIsNone(row)
        cursor.close()

    def test_fetch_many(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL)
        rows = cursor.fetchmany(2)
        self.assertEqual(len(rows), 2)
        self.assertEqual(rows[0][0], "1")
        self.assertEqual(rows[1][0], "2")
        cursor.close()

    def test_fetch_many_empty_sequence_if_no_data(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL + " where nr < 0")
        rows = cursor.fetchmany()
        self.assertListEqual(rows, [])
        cursor.close()

    def test_fetch_many_using_arraysize(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL)
        cursor.arraysize = 2
        rows = cursor.fetchmany()
        self.assertEqual(len(rows), 2)
        self.assertEqual(rows[0][0], "1")
        self.assertEqual(rows[1][0], "2")
        rows = cursor.fetchmany(1)
        self.assertEqual(len(rows), 1)
        cursor.close()

    def test_fetch_all_empty_sequence_if_no_data(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL + " where nr < 0")
        rows = cursor.fetchall()
        self.assertListEqual(rows, [])
        cursor.close()

    def test_fetch_all(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL)
        rows = cursor.fetchall()
        self.assertEqual(len(rows), 5)
        self.assertEqual(len(rows[0]), 4)

    def test_fetch_all_remaining(self):
        cursor = self.client.cursor()
        cursor.execute(SELECT_ALL)
        first = cursor.fetchone()
        self.assertEqual(first[0], "1")
        rows = cursor.fetchall()
        self.assertEqual(len(rows), 4)
        self.assertEqual(rows[0][0], "2")

    def test_execute_positional_parameters(self):
        cursor = self.client.cursor()
        cursor.execute(
            SELECT_ALL + " where nr > ? and comment = ?",
            ["0", "Withdrawal"],
        )
        row = cursor.fetchone()
        self.assertIsNotNone(row)
        cursor.close()

    def test_execute_named_parameters(self):
        cursor = self.client.cursor()
        cursor.execute(
            SELECT_ALL + " where nr > :nr and comment = :comment",
            {"nr": "0", "comment": "Withdrawal"},
        )
        row = cursor.fetchone()
        self.assertIsNotNone(row)
        cursor.close()

    def test_execute_with_null_parameters(self):
        cursor = self.client.cursor()
        cursor.execute(
            SELECT_ALL + " where comment = ?",
            [None],
        )
        row = cursor.fetchone()
        self.assertIsNotNone(row)
        cursor.close()


if __name__ == "__main__":
    unittest.main()
