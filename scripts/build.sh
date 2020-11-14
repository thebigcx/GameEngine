#!/bin/bash

# Use CMake and a Makefile to compile
vendor/premake/premake5 gmake Release
make

echo "Built target in ./bin/"
