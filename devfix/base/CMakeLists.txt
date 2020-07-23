cmake_minimum_required(VERSION 3.0)
set(LIBRARY "devfixbase")

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp *.c *.cpp)
list(FILTER src EXCLUDE REGEX "(test_[a-zA-Z_]+..*$)")  # test-cases are not part of the library
add_library(${LIBRARY} "${src}" lnx/filesystem.cpp)

set_target_properties(${LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${LIBRARY} PRIVATE ${WARNINGS})
