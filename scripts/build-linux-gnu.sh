#!/bin/sh

set -eu

. "$PWD/libxc.ver"

rm -rf "Linux/gnu" "Linux"/libxc-*-linux-gnu-*.zip "Linux"/libxc-*-linux-gnu-*.7z

# Linux x86_64
for ARCH in x86_64 i686; do
	cmake -S .. -B Linux/gnu/${ARCH} \
		-DCMAKE_TOOLCHAIN_FILE=cmake/${ARCH}-linux-gnu.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DLIBXC_INSTALL_PREFIX="../.." \
		-DCMAKE_INSTALL_PREFIX="$PWD/Linux/gnu/${ARCH}/prefix"

	cmake --build Linux/gnu/${ARCH} --config Release --target install

	(
		cd Linux/gnu/${ARCH}/prefix
		zip -9 -r "../../../libxc-${LIBXC_VERSION}-linux-gnu-${ARCH}.zip" .
		7z a -t7z -mx=9 -ms=on "../../../libxc-${LIBXC_VERSION}-linux-gnu-${ARCH}.7z" .
	)
done