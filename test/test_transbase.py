import unittest
from transbase import transbase

sample = ("//localhost:2024/sample", "tbadmin", "")


class TestTransbase(unittest.TestCase):
    def test_connect_error(self):
        client = transbase.connect("//localhost:2024/what", "tbadmin", "")
        self.assertRaises(ValueError)
        self.assertEqual(client.state(), -1)

    def test_connect_success(self):
        client = transbase.connect(*sample)
        self.assertIsNotNone(client)
        self.assertIsInstance(client, transbase.Connection)
        self.assertEqual(client.state(), 0)

    def test_close_connection(self):
        client = transbase.connect(*sample)
        client.close()
        self.assertEqual(client.state(), None)

    def test_execute_query(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        self.assertIsInstance(cursor, transbase.Cursor)
        self.assertIsNotNone(cursor)
        cursor.execute("select * from cashbook")
        row = cursor.fetchone()
        self.assertEqual(cursor.state(), 0)
        self.assertIsNotNone(row)
        cursor.close()

    def test_result_set_meta(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook")
        self.assertEqual(len(cursor.description), 4)
        self.assertEqual(
            cursor.description,
            [["nr", 16644], ["date", 17777], ["amount", 16645], ["comment", 16649]],
        )
        cursor.close()

    def test_fetch_one_row(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select nr, amount, comment from cashbook")
        row = cursor.fetchone()
        self.assertIsNotNone(row)
        self.assertEqual(row, ["1", "100.00", "Withdrawal"])
        cursor.close()

    def test_fetch_one_return_none_if_no_data(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook where id < 0")
        row = cursor.fetchone()
        self.assertIsNone(row)
        cursor.close()

    def test_fetch_many(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook")
        rows = cursor.fetchmany(2)
        self.assertEqual(len(rows), 2)
        self.assertEqual(rows[0][0], "1")
        self.assertEqual(rows[1][0], "2")

    def test_fetch_many_empty_sequence_if_no_data(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook where id < 0")
        rows = cursor.fetchmany()
        self.assertListEqual(rows, [])

    def test_fetch_many_using_arraysize(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook")
        cursor.arraysize = 2
        rows = cursor.fetchmany()
        self.assertEqual(len(rows), 2)
        self.assertEqual(rows[0][0], "1")
        self.assertEqual(rows[1][0], "2")
        rows = cursor.fetchmany(1)
        self.assertEqual(len(rows), 1)

    def test_fetch_all_empty_sequence_if_no_data(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook where nr < 0")
        rows = cursor.fetchall()
        self.assertListEqual(rows, [])

    def test_fetch_all(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook")
        rows = cursor.fetchall()
        self.assertEqual(len(rows), 4)
        self.assertEqual(len(rows[0]), 4)

    def test_fetch_all_remaining(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook")
        first = cursor.fetchone()
        self.assertEqual(first[0], "1")
        rows = cursor.fetchall()
        self.assertEqual(len(rows), 3)
        self.assertEqual(rows[0][0], "2")


if __name__ == "__main__":
    unittest.main()
