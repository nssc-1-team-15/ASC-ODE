FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    python3 \
    python3-dev \
    python3-pybind11 \
    python3-numpy \
    python3-matplotlib \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace