#!/bin/sh

set -eu

bash "$PWD/remove.sh"

mkdir -p Archives

bash "$PWD/libxc-build-src.sh"

bash "$PWD/build-linux.sh"
cp Linux/*.zip Linux/*.7z Archives/

bash "$PWD/build-macos.sh"
cp Darwin/*.zip Darwin/*.7z Archives/

bash "$PWD/build-windows.sh"
cp Windows/*.zip Windows/*.7z Archives/