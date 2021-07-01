import unittest
from transbase import transbase


class TestTransbase(unittest.TestCase):
    def test_connect_error(self):
        transbase.connect("//localhost:2024/what", "tbadmin", "")
        self.assertRaises(ValueError)

    def test_connect_success(self):
        client = transbase.connect("//localhost:2024/sample", "tbadmin", "")
        self.assertIsNotNone(client)
        self.assertIsInstance(transbase.Connection)


if __name__ == "__main__":
    unittest.main()
