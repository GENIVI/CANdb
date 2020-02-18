#!/bin/bash

cmake_command="cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$(pwd)/install"

echo "CMake: $cmake_command"
echo "Working dir: $(pwd)"
ls -la
rm -rf build && mkdir -p build 
rm -rf install && mkdir -p install 
cd build
$cmake_command 
make -j5 install && make test

