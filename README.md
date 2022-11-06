# Automated brewery project - driver

The hobby project intended to automate process of brewing.

This is low-lewer driver part. The other projects are located at: [brewery-host](https://github.com/Michal-Wadowski/brewery-host) and [brewery-android-client](https://github.com/Michal-Wadowski/brewery-android-client)

# Building

To build Orange PI library please type:

```shell
docker image build . -t brewery-driver
container_id=$(docker create brewery-driver:latest)
docker cp $container_id:/driver/brewery_driver.so - > brewery_driver.so
docker rm -v $container_id
```

To build demo library just type:

```shell
make clean && make demo
```
