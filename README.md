# transbase-python

<p align="center">
    <a href="https://badge.fury.io/py/transbase"><img src="https://badge.fury.io/py/transbase.svg" alt="pypi version" height="18"></a>
    <a href="https://github.com/TransactionSoftwareGmbH/transbase-python/actions/workflows/python-verify.yml"><img src="https://github.com/TransactionSoftwareGmbH/transbase-python/actions/workflows/python-verify.yml/badge.svg " alt="test" height="18"></a>
</p>

Python database driver for [Transbase](https://www.transaction.de/produkte/transbase.html)  
based on Transbase/TCI ([Transase C/C++ call interface](https://www.transaction.de/transbase/doku/call-interface-tci.html) )  
implementing Python database api v2.0 ([PEP-249](https://www.python.org/dev/peps/pep-0249/))

|               |       |
| ------------- | ----: |
| **Python**    |   3.9 |
| **Transbase** |   8.x |

## Install

Transbase is available in [PyPi](https://test.pypi.org/project/transbase/)

```
pip install transbase
```

## Example

```python
from transbase import transbase

# connect to transbase database
connection = transbase.connect("//develop.transaction.de:8324/test", "test", "test")

cursor = connection.cursor()

cursor.execute("select no, text, date from test")
# use native python data types in fetch result set (bool, int, float, bytes,...)
# otherwise all values will be fetched as plain "str"
cursor.type_cast = True

row = cursor.fetchone()
print(row)

cursor.close()
connection.close()
```

DML statements (insert, update, delete) are executed similar.  
The number of affected rows can be obtained by `.rowcount`

```python
cursor.execute("insert into test ( text ) values ( '... kind regards' )");
print(cursor.rowcount) # -> 1
```

Query parameters can be passed as second argument

```python
# pass parameters as object matching named parameters
cursor.execute(
    "select no, text, date from test where no >= :no and text like :text",
    { "no": 1, "text": "Hello%" }
)

# or as an array for positional parameters
cursor.execute(
    "select no, text, date from test where no >= ? and text like insensitive ?",
    [ 2, "%kind%" ]
)
```

## Troubleshooting

### TCI SDK not found

If no prebuild binary wheel could be found on PyPi for your operating system or Python version  
you can simply download it after installation by running the python script  
(added with installation of this package)

```
install_tci
```

## Contribution

VS-Code Editor with python extension is recommended.

### Development

- install python 3.9 (TODO: -> 3.11)
- it is recommended to install and use virtualenv
  `py -m pip install --user virtualenv`
- install required dependencies
  `py -m pip install flake8 coverage requests`
- download tci lib `py setup.py get_lib`

### Test

- `py -m unittest` or use vs-code test explorer

### Build

Source distribution and wheel including tci sdk

- `py setup.py sdist [bdist_wheel]`

#### Manual Release

Linux wheels need to be renamed matching [PEP-600](https://www.python.org/dev/peps/pep-0600/)

To publish manually to pypi (or testP) run

- `twine upload [--repository testpypi] dist/*`
