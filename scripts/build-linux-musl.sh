#!/bin/sh

set -eu

. "$PWD/libxc.ver"

rm -rf "Linux/musl" "Linux"/libxc-*-linux-musl-*.zip "Linux"/libxc-*-linux-musl-*.7z

# Darwin i386 x86_64
for ARCH in x86_64 i386; do
	cmake -S .. -B Linux/musl/${ARCH} \
		-DCMAKE_TOOLCHAIN_FILE=cmake/${ARCH}-linux-musl.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DLIBXC_INSTALL_PREFIX="../.." \
		-DCMAKE_INSTALL_PREFIX="$PWD/Linux/musl/${ARCH}/prefix"

	cmake --build Linux/musl/${ARCH} --config Release --target install

	(
		cd Linux/musl/${ARCH}/prefix
		zip -9 -r "../../../libxc-${LIBXC_VERSION}-linux-musl-${ARCH}.zip" .
		7z a -t7z -mx=9 -ms=on "../../../libxc-${LIBXC_VERSION}-linux-musl-${ARCH}.7z" .
	)
done