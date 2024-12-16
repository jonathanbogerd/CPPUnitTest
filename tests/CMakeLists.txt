project(${PROJECT_NAME})


add_executable(tests
    MathTest.cpp
)

target_link_libraries(
  tests
  CPPUnitTest
)
