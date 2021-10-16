FROM python:3.9.7-bullseye

ENV DB_URL="localhost:2024/sample"
ENV DB_USER="dbadmin"
ENV DB_PASSWORD=""

ADD transbase ./transbase
ADD test ./test
ADD setup.py ./setup.py

RUN python -m pip install --upgrade pip
RUN python -m pip install requests twine
RUN python setup.py get_lib

RUN python -m unittest

CMD bash