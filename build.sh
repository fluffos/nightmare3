#!/bin/bash
set -euo pipefail

mkdir -p build
cd build

rm -rf *
cmake ../driver
make -j4 install

cd ..
