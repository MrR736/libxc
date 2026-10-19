#!/bin/sh

set -eu

rm -rf Linux

bash "$PWD/build-linux-gnu.sh"
bash "$PWD/build-linux-musl.sh"