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
        # self.assertIsNotNone(row)


if __name__ == "__main__":
    unittest.main()
