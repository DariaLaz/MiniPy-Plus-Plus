#!/usr/bin/env bash

set -e

OUTPUT="main.exe"

CPP_FILES=$(find . \
    -path "./build" -prune -o \
    -path "./Tests" -prune -o \
    -name "*.cpp" -print)

echo "Compiling:"
echo "$CPP_FILES"
echo

g++ \
    $CPP_FILES \
    -I. \
    -std=c++17 \
    -Wall \
    -Wextra \
    -o "$OUTPUT"

echo
echo "Build successful: $OUTPUT"