#!/bin/bash
find ./src -iname "*.cpp" | xargs clang-tidy -p ./build
