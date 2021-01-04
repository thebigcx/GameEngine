#!/bin/bash

# Use Premake and a Makefile to compile
vendor/premake/premake5 gmake
make config=Release

echo "Built release target in ./bin/"
