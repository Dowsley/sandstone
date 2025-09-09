#!/bin/bash

# Sandstone build script using vcpkg
set -e

VCPKG_ROOT="/opt/vcpkg"

echo "Building Sandstone with vcpkg..."

# Clean previous build
rm -rf build

# Configure with vcpkg
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build

echo "Build complete! Run with: ./build/sandstone"
