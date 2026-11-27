FROM ubuntu:26.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    python3 \
    python3-dev \
    python3-venv \
    python3-pybind11 \
    python3-numpy \
    python3-matplotlib \
    nodejs \
    npm \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Ubuntu 26.04 currently packages npm 9; MyST requires npm 10 or newer.
RUN /usr/bin/npm install --global --prefix=/usr/local npm@10 \
    && npm --version

# Jupyter Book lives in its own environment; plotting uses Ubuntu's system Python.
RUN python3 -m venv /opt/jupyterbook-venv \
    && /opt/jupyterbook-venv/bin/pip install --no-cache-dir "jupyter-book>=2"

WORKDIR /workspace
