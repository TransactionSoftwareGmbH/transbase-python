from setuptools import setup, Command
import requests
import tarfile
import platform
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
        version = "8.4.1"
        os_platform = platform.system().lower()
        os_arch = "x64" if sys.maxsize > 2 ** 32 else "x86"
        url = f"https://www.transaction.de/transbase/{version}/{os_platform}_{os_arch}/sdk/transbase-{version}_{os_platform}_{os_arch}_sdk.tgz"
        print("downloading from ", url)
        response = requests.get(url, stream=True)
        file = tarfile.open(fileobj=response.raw, mode="r|gz")
        file.extractall(path=".")


setup(
    name="transbase",
    packages=["transbase"],
    has_ext_modules=lambda: True,
    package_data={
        "transbase": [f"../lib/{get_lib_name()}"],
    },
    version="0.0.2",
    description="Python Transbase Client",
    url="https://www.transaction.de",
    author="Daniel Loibl",
    author_email="daniel.loibl@gmail.com",
    license="MIT",
    test_suite="test",
    cmdclass={
        "get_lib": DownloadSdkCommand,
    },
    python_requires=">=3.9",
)
