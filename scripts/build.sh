#!/bin/bash

# Create temporary directories for building
mkdir -p build
mkdir -p bin

# Use CMake and a Makefile to compile
cd build
cmake ..
make

# Copy resources to the direcory with the executable
cd ..
mkdir bin/sandbox/res
mkdir bin/shaders
cp res/* bin/res
cp shaders/* bin/shaders

echo "Built target in ./bin/"
