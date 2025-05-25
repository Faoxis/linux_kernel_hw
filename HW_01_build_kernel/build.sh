#!/bin/bash
set -euo pipefail
# подставляем количество ядер контейнера (=ядер хоста)
/usr/bin/make -j"$(nproc)" deb-pkg 2>/out/error.log
cp -v ../*.deb /out/
