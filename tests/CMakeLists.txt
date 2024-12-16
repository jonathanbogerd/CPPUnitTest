project(${PROJECT_NAME})


add_executable(tests
    MathTest.cpp
    PerformanceTest.cpp
    StringTest.cpp
)

target_link_libraries(
  tests
  CPPUnitTest
)
