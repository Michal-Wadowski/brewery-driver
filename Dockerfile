FROM ubuntu:18.04

COPY . /driver

RUN apt update
RUN apt install -y git gcc-arm-linux-gnueabihf make python3

RUN git clone https://github.com/orangepi-xunlong/wiringOP
WORKDIR wiringOP

RUN git apply /driver/custom.patch

WORKDIR wiringPi

RUN make && make install

WORKDIR ../devLib

RUN make && make install

WORKDIR /driver

