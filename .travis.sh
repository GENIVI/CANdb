#!/bin/bash

cmake_command="cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$(pwd)/install"

if [ "$CANDB__WITH_COVERAGE" == "ON" ]; then
    cmake_command="$cmake_command -DCANDB__WITH_COVERAGE=ON"
fi

echo "CMake: $cmake_command"
echo "Working dir: $(pwd)"
ls -la
rm -rf build && mkdir -p build 
rm -rf install && mkdir -p install 
cd build
$cmake_command 
make -j5 install && make test

