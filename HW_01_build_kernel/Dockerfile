# docker/kernel-builder/Dockerfile
FROM debian:12-slim
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential bc bison flex libssl-dev libelf-dev libncurses-dev \
        dwarves python3 python3-distutils rsync kmod fakeroot lsb-release \
        zstd liblz4-tool cpio git openssl && \
    rm -rf /var/lib/apt/lists/*

COPY build.sh /build.sh

WORKDIR /usr/src/linux
ENTRYPOINT ["/build.sh"]

# Сам build.sh положите рядом с Dockerfile
