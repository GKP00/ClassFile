cmake_minimum_required(VERSION 3.14)
project(ClassFileTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(ParseTest ParseTest.cpp)
target_link_libraries(ParseTest ClassFile GTest::gtest_main)

file(CREATE_LINK "${PROJECT_SOURCE_DIR}/res" "${PROJECT_BINARY_DIR}/res" SYMBOLIC)
target_compile_definitions(ParseTest PRIVATE RES_DIR="${PROJECT_BINARY_DIR}/res")

include(GoogleTest)
gtest_discover_tests(ParseTest)

