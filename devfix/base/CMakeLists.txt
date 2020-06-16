cmake_minimum_required(VERSION 3.0)
set(LIBRARY "devfixbase")
add_library(${LIBRARY} error/baseexception.h error/interruptedexception.h memory.h platform.h error/timeoutexception.h io/inputstream.h io/source.cpp io/source.h io/sink.cpp io/sink.h io/iotypes.h error/namespace.h strout.h interpolation.h time.h)
set_target_properties(${LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${LIBRARY} PRIVATE ${WARNINGS})

if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_custom_target(PrintBuildType ALL
        ${CMAKE_COMMAND} -E cmake_echo_color --red "### cmake build type: ${CMAKE_BUILD_TYPE} ###"
        COMMENT "Library Build Type" )
    add_dependencies(${LIBRARY} PrintBuildType )
endif()
