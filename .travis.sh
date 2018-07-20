#!/bin/bash

cmake_command="cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"

if [ "$WITH_COVERAGE" == "ON" ]; then
    cmake_command="$cmake_command -DWITH_COVERAGE=ON"
fi

echo "CMake: $cmake_command"
echo "ci_env: $ci_env"
docker run \
     $ci_env -e WITH_COVERAGE -u cds -e CC -e CXX -e DISPLAY=:10 --privileged --cap-add=ALL \
     -v `pwd`:/home/sources rkollataj/candevstudio /bin/bash \
     -c "cd /home/sources && rm -rf build && mkdir -p build && cd build &&
        $cmake_command &&
        make -j5 &&
        make test"
