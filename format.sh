#!/bin/bash
find ./src ./tests ./examples -iname "*.cpp" | xargs clang-format -i
