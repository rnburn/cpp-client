#!/bin/bash

set -e

BUILD_IMAGE="ubuntu:17.10"
[[ -z "${JAEGER_DOCKER_BUILD_DIR}" ]] && export JAEGER_DOCKER_BUILD_DIR=/tmp/jaeger-docker-build

mkdir -p "${JAEGER_DOCKER_BUILD_DIR}"
docker run -v "$PWD":/src \
           -v "${JAEGER_DOCKER_BUILD_DIR}":/build \
           -w /src -it "$BUILD_IMAGE" /bin/bash -lc "./plugin/do_build_linux_plugin.sh"
