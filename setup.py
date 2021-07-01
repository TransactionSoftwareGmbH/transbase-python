from setuptools import find_packages, setup

setup(
    name="transbase",
    packages=find_packages(include=["transbase"]),
    version="0.0.1",
    description="Python Transbase Client",
    author="Daniel Loibl",
    license="MIT",
    test_suite="test",
)
