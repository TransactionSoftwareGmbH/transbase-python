import requests
import tarfile
import platform
import sys
import os.path


def download_tci(
    path=os.path.dirname(os.path.abspath(__file__)),
    version="8.4.1",
):
    os_platform = platform.system().lower()
    os_arch = "x64" if sys.maxsize > 2 ** 32 else "x86"
    url = f"https://www.transaction.de/transbase/{version}/{os_platform}_{os_arch}/sdk/transbase-{version}_{os_platform}_{os_arch}_sdk.tgz"
    print(f"downloading from {url} into {os.path.abspath(path)}")
    response = requests.get(url, stream=True)
    file = tarfile.open(fileobj=response.raw, mode="r|gz")
    file.extractall(path)
