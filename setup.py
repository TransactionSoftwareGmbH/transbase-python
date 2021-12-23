from setuptools import setup, Command
from transbase.install import download_tci
from pathlib import Path
import sys


def get_lib_name():
    if sys.platform.startswith("win"):
        return "tci.dll"
    elif sys.platform.startswith("darwin"):
        return "libtci.dylib"
    else:
        return "libtci.so"


class DownloadSdkCommand(Command):
    """download tci sdk command."""

    description = "download tci sdk"
    user_options = []

    def initialize_options(self):
        # nothing todo
        pass

    def finalize_options(self):
        # nothing todo
        pass

    def run(self):
        download_tci(".")


setup(
    name="transbase",
    packages=["transbase"],
    scripts=["scripts/install_tci"],
    has_ext_modules=lambda: True,
    package_data={
        "transbase": [f"../lib/{get_lib_name()}"],
    },
    version="0.0.5",
    description="Python Transbase Client",
    long_description=(Path(__file__).parent / "README.md").read_text(),
    long_description_content_type="text/markdown",
    url="https://www.transaction.de",
    author="Daniel Loibl",
    author_email="loibl@transaction.de",
    license="MIT",
    test_suite="test",
    cmdclass={
        "get_lib": DownloadSdkCommand,
    },
    python_requires=">=3.9",
    install_requires=["requests"],
)
