# transbase-python

<p align="center">
    <a href="https://badge.fury.io/py/transbase"><img src="https://badge.fury.io/py/transbase.svg" alt="pypi version" height="18"></a>
    <a href="https://github.com/TransactionSoftwareGmbH/transbase-python/actions/workflows/python-verify.yml"><img src="https://github.com/TransactionSoftwareGmbH/transbase-python/actions/workflows/python-verify.yml/badge.svg " alt="test" height="18"></a>
</p>

A python client for [transbase](https://www.transaction.de/loesungen/transbase-ressourcenoptimierte-hochleistungsdatenbank)
based on tci.

## Development

- install python 3.9
- it is recommended to install virtualenv
  `py -m pip install --user virtualenv`
- Install required dependencies
  `py -m pip install flake8 coverage requests`
- Download tci lib `py setup.py get_lib`

## Test

- `py -m unittest`
- or use vs-code test explorer

## Build

### Wheel including tci sdk

- `py setup.py bdist_wheel`
