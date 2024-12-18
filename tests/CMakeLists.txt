project(${PROJECT_NAME})


add_executable(tests
    MathTest.cpp
    SetupTest.cpp
    StringTest.cpp
)

target_link_libraries(
  tests
  CPPUnitTest
)
