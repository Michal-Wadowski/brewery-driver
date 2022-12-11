# Automated brewery project - driver

The hobby project intended to automate process of brewing.

This is low-lewer driver part. The other projects are located at: [brewery-host](https://github.com/Michal-Wadowski/brewery-host)

# Building

To build Orange PI libraries please type:

```shell
docker image build . -f docker/Dockerfile-libs -t minipc:32000/orangepi-libs && docker push minipc:32000/orangepi-libs:latest
```

To build demo library just type:

```shell
make clean && make demo
```
