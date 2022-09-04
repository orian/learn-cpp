# Get the base Ubuntu image from Docker Hub
FROM ubuntu:20.04

# Update apps on the base image
RUN apt-get -y update && apt-get install -y
RUN DEBIAN_FRONTEND=noninteractive TZ=Europe/Warsaw apt-get install -y build-essential cmake git wget apt-transport-https curl gnupg openjdk-11-jdk
RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel-archive-keyring.gpg && \
    mv bazel-archive-keyring.gpg /usr/share/keyrings && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && \
    apt-get update &&  \


RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test &&  \
    apt install -y gcc-11 g++-11  \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 30 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 30 && \
    update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-11 30 && \
    update-alternatives --set cc /usr/bin/gcc && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30

# Specify the working directory
WORKDIR /workdir
