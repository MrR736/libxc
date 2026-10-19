#!/bin/sh

bash "$PWD/remove.sh"

. "$PWD/libxc.ver"

(
	cd ..
	zip -9 -r "scripts/libxc-${LIBXC_VERSION}-src.zip" .
)

mkdir -p Archives

mv "libxc-${LIBXC_VERSION}-src.zip" Archives/

