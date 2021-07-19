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

    def test_fetch_data(self):
        client = transbase.connect(*sample)
        cursor = client.cursor()
        cursor.execute("select * from cashbook")
        row = cursor.fetchone()
        self.assertIsNotNone(row)
        self.assertEqual(
            row,
            [1, "2021-18-7", 3, "ommet"],
        )
        cursor.close()


if __name__ == "__main__":
    unittest.main()
