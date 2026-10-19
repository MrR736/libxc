#!/bin/sh

set -eu

. "$PWD/libxc.ver"

rm -rf Darwin

# Darwin arm64 arm64e x86_64 x86_64h
for ARCH in arm64 arm64e x86_64 x86_64h; do
	cmake -S .. -B Darwin/${ARCH} \
		-DCMAKE_TOOLCHAIN_FILE=cmake/MacOSXClang.cmake \
		-DMACOSX_SDK_VERSION=14.5 \
		-DMACOSX_TARGET=${ARCH}-apple-darwin \
		-DCMAKE_BUILD_TYPE=Release \
		-DLIBXC_INSTALL_PREFIX="../.." \
		-DCMAKE_INSTALL_PREFIX="$PWD/Darwin/${ARCH}/prefix"

	cmake --build Darwin/${ARCH} --config Release --target install

	(
		cd Darwin/${ARCH}/prefix
		zip -9 -r "../../libxc-${LIBXC_VERSION}-Darwin-${ARCH}.zip" .
		7z a -t7z -mx=9 -ms=on "../../libxc-${LIBXC_VERSION}-Darwin-${ARCH}.7z" .
	)
done