cmake_minimum_required(VERSION 3.28)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD 0e5b6991-d74f-4b3d-a41c-cf096e0b2508)
project(CPPUnitTest VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_MODULE_STD 1)

# Set C++ standards and compiler flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -march=x86-64-v3 --std=c++23")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=x86-64-v3 --std=c++23 -DNDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define compile definitions
add_compile_definitions(CPPUNITTEST_MAX_THREADS=2)
add_compile_definitions(EXCLUDED_GROUPS="")
add_compile_definitions(EXCLUDED_TESTS="")

# Create the library
add_library(CPPUnitTest)

target_sources(CPPUnitTest
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    src/CPPUnitTest.cpp
    src/TestManager.cpp
    src/TestRegister.cpp
    src/Assert.cpp
)

# Define install locations
include(GNUInstallDirs)

# Set target properties for exporting
set_target_properties(CPPUnitTest PROPERTIES
    VERSION ${PROJECT_VERSION}
)

install(TARGETS CPPUnitTest
    EXPORT CPPUnitTestTargets
    FILE_SET cxx_modules DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CPPUnitTest
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # For executables, if applicable
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Shared libraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # Static libraries
)


# Install the CMake configuration files for find_package
install(EXPORT CPPUnitTestTargets
    FILE CPPUnitTestTargets.cmake
    NAMESPACE CPPUnitTest::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CPPUnitTest
)

# Create the configuration file for find_package
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/CPPUnitTestConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CPPUnitTestConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CPPUnitTest
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CPPUnitTestConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CPPUnitTestConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CPPUnitTestConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CPPUnitTest
)

# Options for building examples and tests
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
