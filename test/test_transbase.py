import os
from transbase.error import DatabaseError
import unittest
from transbase import transbase
import uuid

db_url = os.getenv("DB_URL", "//develop.transaction.de:8324/test")
db_user = os.getenv("DB_USER", "test")
db_password = os.getenv("DB_PASSWORD", "test")
db = (db_url, db_user, db_password)

TABLE = "cashbook_py_" + str(uuid.uuid4())[0:7]
TABLE_DATA_TYPES_TEST = TABLE + "_data_types"

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
            cursor.execute(f"drop table {TABLE_DATA_TYPES_TEST};")
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

    def test_multiple_cursor_connections_assignments(self):
        client = transbase.connect(*db)
        cursor = client.cursor()
        cursor.execute(SELECT_ALL)
        cursor.close()
        cursor = client.cursor()
        cursor.close()
        client.close()

        client = transbase.connect(*db)
        cursor = client.cursor()
        cursor.execute(SELECT_ALL)
        cursor.close()
        client.close()

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

    def test_fetch_type_cast(self):
        self.prepare_table_all_types()
        cursor = self.client.cursor()

        cursor.execute(f"select * from {TABLE_DATA_TYPES_TEST}")
        row = cursor.fetchone()
        self.assertEqual(
            row,
            [
                "120",
                "32000",
                "2000111222",
                "4000111222333",
                "5.20",
                "555.22",
                "0110",
                "Clob",
                "Varchar(*)",
                "Char(*)",
                "String",
                "0110",
                "0110110",
                "0110",
                "true",
                "2002-12",
                "2002-12-24",
                "17:35:10",
                "2002-12-24 17:35:10.250",
                "2-06",
                "2:12:35",
            ],
        )

        cursor.type_cast = True
        cursor.execute(f"select * from {TABLE_DATA_TYPES_TEST}")
        row = cursor.fetchone()
        self.assertEqual(
            row,
            [
                int(120),
                int(32000),
                int(2000111222),
                int(4000111222333),
                float(5.2),
                float(555.22),
                bytes([1, 16]),
                "Clob",
                "Varchar(*)",
                "Char(*)",
                "String",
                bytes([1, 16]),
                "0110110",
                "0110",
                True,
                "2002-12",
                "2002-12-24",
                "17:35:10",
                "2002-12-24 17:35:10.250",
                "2-06",
                "2:12:35",
            ],
        )
        cursor.close()

    def test_parameter_type_cast(self):
        cursor = self.client.cursor()
        cursor.execute(
            f"select * from {TABLE_DATA_TYPES_TEST} where a > ? and f > ? and o = ?",
            [5, 1.1, True],
        )
        self.assertEqual(0, cursor.state())
        cursor.execute(
            f"select * from {TABLE_DATA_TYPES_TEST} where a > :a and f > :f and o = :o",
            {"a": 5, "f": 1.1, "o": True},
        )
        self.assertEqual(0, cursor.state())
        cursor.close()

    def test_insert_type_cast(self):
        cursor = self.client.cursor()
        cursor.execute(
            f"insert into {TABLE_DATA_TYPES_TEST} values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);",
            [
                int(42),
                int(4200),
                int(2000111242),
                int(4000111222342),
                float(42.2),
                float(555.42),
                bytes([1, 2, 3]),
                None,
                "Varchar(*)",
                "Char(*)",
                "String",
                bytes([1, 2, 3]),
                None,
                None,
                True,
                None,
                None,
                None,
                None,
                None,
                None,
            ],
        )
        self.assertEqual(0, cursor.state())
        cursor.close()

    def prepare_table_all_types(self):
        cursor = self.client.cursor()
        cursor.execute(
            f"""create table {TABLE_DATA_TYPES_TEST}
           (
            a TINYINT,
            b SMALLINT,
            c INTEGER,
            d BIGINT,
            e NUMERIC(5,2),
            f DECIMAL(5,2),
            g BLOB,
            h CLOB,
            i VARCHAR(*),
            j CHAR(*) ,
            k STRING,
            l BINCHAR (*),
            m BITS (*),
            n BITS2 (*),
            o BOOL,
            p DATETIME[YY:MO],
            q DATE,
            r TIME,
            s TIMESTAMP,
            t TIMESPAN[YY:MO],
            u INTERVAL HOUR TO SECOND
           );"""
        )

        cursor.execute(
            f"""
        insert into {TABLE_DATA_TYPES_TEST} values (
          120,
          32000,
          2000111222,
          4000111222333,
          5.2,
          555.22,
          0x0110,
          'Clob',
          'Varchar(*)',
          'Char(*)' ,
          'String',
          0x0110,
          0b0110110,
          0b0110,
          TRUE,
          DATETIME(2002-12),
          DATE '2002-12-24',
          TIME '17:35:10',
          TIMESTAMP '2002-12-24 17:35:10.025',
          TIMESPAN[YY:MO](2-6),
          INTERVAL '2:12:35' HOUR TO SECOND
          );
        """
        )
        cursor.close()


if __name__ == "__main__":
    unittest.main()
