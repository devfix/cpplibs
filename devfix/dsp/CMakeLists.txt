cmake_minimum_required(VERSION 3.0)
set(LIBRARY "devfixdsp")

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp *.c *.cpp)
list(FILTER src EXCLUDE REGEX "(test_[a-zA-Z_]+..*$)")  # test-cases are not part of the library
add_library(${LIBRARY} "${src}")

set_target_properties(${LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${LIBRARY} devfixbase)
target_compile_options(${LIBRARY} PRIVATE ${WARNINGS})
