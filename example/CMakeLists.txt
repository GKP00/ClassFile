project(ClassFileExamples)

add_executable(readclass "readclass.cpp")
target_link_libraries(readclass PUBLIC ClassFile)

add_executable(dupeclass "dupeclass.cpp")
target_link_libraries(dupeclass PUBLIC ClassFile)
