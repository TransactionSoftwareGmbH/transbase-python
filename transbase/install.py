import requests
import tarfile
import platform
import sys
import os


def download_tci(
    path=os.path.dirname(os.path.abspath(__file__)),
    version="8.4",
):
    os_platform = platform.system().lower()
    os_arch = "x64" if sys.maxsize > 2**32 else "x86_64"
    url = f"https://www.transaction.de/downloads/transbase/{version}/{os_platform}/{os_arch}/transbase_dev.tar.Z"
    print(f"xdownloading from {url} into {os.path.abspath(path)}")
    response = requests.get(url, stream=True)
    with tarfile.open(fileobj=response.raw, mode="r|gz") as tar:
        for member in tar:
            if member.name.find("tci") > -1:
                file = tar.extractfile(member)
                if file is None:
                    continue
                outfile = os.path.join(path, "lib", os.path.basename(member.name))
                os.makedirs(os.path.join(path, "lib"), exist_ok=True)
                with open(outfile, "wb") as output:
                    output.write(file.read())
