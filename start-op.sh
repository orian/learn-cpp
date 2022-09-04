#!/usr/bin/env sh

echo "100000000\n" > /tmp/data.txt

./cmake-build-release/benchmark_op < /tmp/data.txt
