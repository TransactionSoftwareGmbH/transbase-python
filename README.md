# transbase-python

<p align="center">
    <a href="https://badge.fury.io/py/transbase"><img src="https://badge.fury.io/py/transbase.svg" alt="pypi version" height="18"></a>
    <a href="https://github.com/TransactionSoftwareGmbH/transbase-python/actions/workflows/python-verify.yml"><img src="https://github.com/TransactionSoftwareGmbH/transbase-python/actions/workflows/python-verify.yml/badge.svg " alt="test" height="18"></a>
</p>

A python client for [transbase](https://www.transaction.de/loesungen/transbase-ressourcenoptimierte-hochleistungsdatenbank)
based on tci implementing python database api v2.0 ([PEP-249](https://www.python.org/dev/peps/pep-0249/))

|               |       |
| ------------- | ----: |
| **Python**    |   3.9 |
| **Transbase** | 8.4.1 |

## Install

Transbase is available in [PyPi](https://test.pypi.org/project/transbase/)

```
pip install transbase
```

## Example

```python
from transbase import transbase

# change to your transbase connection
client = transbase.connect("//localhost:8024/dbtest", "admin", "admin")

cursor = client.cursor()

cursor.execute("select * from systable")
row = cursor.fetchone()
print(row)

cursor.close()
client.close()
```

DML statements (insert, update and delete) are executed similar. The number of affected rows can be obtained by `.rowcount`

```python
cursor.execute("insert into cashbook values (42, default, 100, 'INSERT');");
print(cursor.rowcount) # -> 1
```

Query parameters can be passed as second argument

```python
# pass parameters as object matching named parameters
cursor.execute(
    "select * from cashbook where nr >= :nr and comment like :comment",
    {"nr": "1", "comment": "Lu%"},
)

# or as an array for positional parameters
cursor.execute(
    "select * from cashbook where nr >= ? and comment like ?",
    ["1", "Lu%"]
)
```

## Troubleshooting

### TCI SDK not found

If no prebuild binary wheel could be found on PyPi for your os or python version you can simply download it after installation by
running the python script (added with installation of this package)

```
install_tci
```

## Contribution

VS-Code Editor with python extension is recommended.

### Development

- install python 3.9 (TODO: -> 3.11)
- it is recommended to install and use virtualenv
  `py -m pip install --user virtualenv`
- Install required dependencies
  `py -m pip install flake8 coverage requests`
- Download tci lib `py setup.py get_lib`

### Test

- `py -m unittest` or use vs-code test explorer

### Build

Source distribution and wheel including tci sdk

- `py setup.py sdist bdist_wheel`

#### Manual Release

Linux wheels need to be renamed matching [PEP-600](https://www.python.org/dev/peps/pep-0600/)

To publish manually to pypi (or testP) run

- `twine upload [--repository testpypi] dist/*`
