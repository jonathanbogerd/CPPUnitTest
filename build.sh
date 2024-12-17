#!/bin/bash
# Set the build mode (Release or Debug, for example)
BuildMode=${1:-Release} # Default to Release if no argument is provided

export CXXFLAGS=-stdlib=libc++
# Create the build directory
mkdir -p ./build/${BuildMode}

# Run CMake to configure the project
cmake -S . -B ./build/${BuildMode} \
      -D CMAKE_BUILD_TYPE=${BuildMode} \
      -G Ninja

# Build the project with Ninja
cmake --build ./build/${BuildMode} --config ${BuildMode}
mv ./build/${BuildMode}/compile_commands.json ./build
cd build/${BuildMode}