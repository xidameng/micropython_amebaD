FROM ubuntu:16.04

RUN apt-get update && \
    apt-get install -y build-essential cmake flex bison wget bzip2 lib32z1 make python python3 gawk && \
    cd /root && \
    wget https://launchpad.net/gcc-arm-embedded/4.8/4.8-2014-q3-update/+download/gcc-arm-none-eabi-4_8-2014q3-20140805-linux.tar.bz2 && \
    tar xvf gcc-arm-none-eabi-4_8-2014q3-20140805-linux.tar.bz2

ENV PATH $PATH:/root/gcc-arm-none-eabi-4_8-2014q3/bin
