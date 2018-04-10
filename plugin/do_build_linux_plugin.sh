#!/bin/bash

set -e

# Setup build environment
apt-get update 
apt-get install --no-install-recommends --no-install-suggests -y \
                build-essential \
                cmake \
                ca-certificates

# Build jaeger
mkdir -p /cmake-build
cd /cmake-build
cat <<EOF > export.map
{
  global:
    extern "C++" {
      OpenTracingMakeTracerFactory;
      opentracing::*;
      jaegertracing::*;
    };
  local: *;
};
EOF
cmake -DBUILD_TESTING=OFF \
      -DCMAKE_CXX_FLAGS="-march=x86-64" \
      -DCMAKE_C_FLAGS="-march=x86-64" \
      -DCMAKE_SHARED_LINKER_FLAGS="-static-libstdc++ -static-libgcc -Wl,--version-script=${PWD}/export.map" \
      /src
make VERBOSE=1 && make install
TARGET_DIR=/build/linux/amd64/
mkdir -p "${TARGET_DIR}"
cp /usr/local/lib/libjaegertracing.so "${TARGET_DIR}"/libjaegertracing_plugin.so
