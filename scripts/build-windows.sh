#!/bin/sh

set -eu

. "$PWD/libxc.ver"

rm -rf Windows

# Windows x64
cmake -S .. -B Windows/x64 \
	-DCMAKE_TOOLCHAIN_FILE=cmake/x86_64-w64-mingw32.cmake \
	-DCMAKE_BUILD_TYPE=Release \
	-DLIBXC_INSTALL_PREFIX="../.." \
	-DCMAKE_INSTALL_PREFIX="$PWD/Windows/x64/prefix"

cmake --build Windows/x64 --config Release --target install

(
	cd Windows/x64/prefix
	zip -9 -r "../../libxc-${LIBXC_VERSION}-win64.zip" .
	7z a -t7z -mx=9 -ms=on "../../libxc-${LIBXC_VERSION}-win64.7z" .
)

# Windows x86
cmake -S .. -B Windows/x86 \
	-DCMAKE_TOOLCHAIN_FILE=cmake/i686-w64-mingw32.cmake \
	-DCMAKE_BUILD_TYPE=Release \
	-DLIBXC_INSTALL_PREFIX="../.." \
	-DCMAKE_INSTALL_PREFIX="$PWD/Windows/x86/prefix"

cmake --build Windows/x86 --config Release --target install

(
	cd Windows/x86/prefix
	zip -9 -r "../../libxc-${LIBXC_VERSION}-win32.zip" .
	7z a -t7z -mx=9 -ms=on "../../libxc-${LIBXC_VERSION}-win32.7z" .
)