cmake_minimum_required(VERSION 3.28)
project(CPPUnitTest VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -march=x86-64-v3 --std=c++23")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=x86-64-v3 --std=c++23 -DNDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCPPUNITTEST_MAX_THREADS=2)

add_library(CPPUnitTest)

target_sources(CPPUnitTest
PUBLIC
FILE_SET cxx_modules TYPE CXX_MODULES FILES
src/CPPUnitTest.cpp
src/TestManager.cpp
src/TestRegister.cpp
src/Assert.cpp
)

option(BUILD_EXAMPLES "Build example executables" ON)
if (BUILD_EXAMPLES)
    message(STATUS "Building examples...")
    add_subdirectory(examples)
endif()

option(BUILD_TESTS "Build library tests" ON)
if (BUILD_TESTS)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
endif()

