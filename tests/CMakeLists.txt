project(${PROJECT_NAME})


add_executable(tests
    MathTest.cpp
    PerformanceTest.cpp
)

target_link_libraries(
  tests
  CPPUnitTest
)
