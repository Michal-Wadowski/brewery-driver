FROM ubuntu:18.04

RUN apt update
RUN apt install -y git gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf make python3 libbluetooth-dev

RUN git clone https://github.com/orangepi-xunlong/wiringOP
WORKDIR wiringOP

COPY ./custom.patch /custom.patch 

RUN git apply /custom.patch

WORKDIR wiringPi

RUN make && make install

WORKDIR ../devLib

RUN make && make install

COPY . /driver

WORKDIR /driver

RUN make clean && make BLUETOOTH=1 REAL_IO=1
