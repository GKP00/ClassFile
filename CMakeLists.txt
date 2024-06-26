cmake_minimum_required(VERSION 3.7)

project(ClassFile)
set(CMAKE_CXX_STANDARD 17)

add_compile_options("-pedantic")
add_compile_options("-Wall")

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG f5e54359df4c26b6230fc61d38aa294581393084 #v10.1.1
  )

FetchContent_MakeAvailable(fmt)


add_library(ClassFile "src/Parser.cpp"
                      "src/Serializer.cpp" 
                      "src/Instruction.cpp" 
                      "src/ConstantPool.cpp" 
                      "src/ClassFile.cpp" 
                      "src/Attribute.cpp"
                      "src/OpCodes.cpp"
                      "src/Misc.cpp")

target_include_directories(ClassFile PRIVATE "src")
target_include_directories(ClassFile PUBLIC "include")

target_link_libraries(ClassFile PRIVATE fmt)

option(BUILD_EXAMPLES "build examples" OFF)
if(BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

option(BUILD_TESTS "build tests" OFF)
if(BUILD_TESTS)
  add_subdirectory(test)
endif()
