cmake_minimum_required(VERSION 3.0)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif ()

project(cpplibs CXX)

set(ENABLE_GOOGLETEST 1)
set(CMAKE_VERBOSE_MAKEFILE false)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D ENABLE_GOOGLETEST=${ENABLE_GOOGLETEST}")

message(STATUS "cmake build type: ${CMAKE_BUILD_TYPE}")

set(WARNINGS
        -pedantic
        -Wall
        -Wextra
        -Wcast-align
        -Wcast-qual
        -Wctor-dtor-privacy
        -Wdisabled-optimization
        -Wformat=2
        -Winit-self
        -Wlogical-op
        -Wmissing-declarations
        -Wmissing-include-dirs
        -Wnoexcept
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wshadow
        -Wsign-conversion
        -Wsign-promo
        -Wstrict-null-sentinel
        -Wstrict-overflow=5
        -Wswitch-default
        -Wundef
        -Wno-unused
        )
set(WARNINGS ${WARNINGS}
        -Wconversion
        -Weffc++
        -Winline
        -Winvalid-pch
        -Wmissing-format-attribute
        -Wnormalized=nfc
        #-Wpadded
        -Wstack-protector
        -Wswitch-enum
        -Wunsafe-loop-optimizations
        )
#set(WARNINGS ${WARNINGS} -Werror)

add_subdirectory(devfix/base)
add_subdirectory(devfix/net)
add_subdirectory(devfix/dsp)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/docs)

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )

    add_custom_target(
            documentation
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API documentation... "
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    #add_custom_target(
    #        documentation-latex
    #        COMMAND ${CMAKE_COMMAND} -E echo_append "Building latex documentation... "
    #        COMMAND make
    #        COMMAND ${CMAKE_COMMAND} -E echo "Done."
    #        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/latex
    #)
    #add_dependencies(documentation-latex documentation)

endif(DOXYGEN_FOUND)


file(GLOB_RECURSE src_tests ${CMAKE_CURRENT_SOURCE_DIR} "test_*.cpp")
add_executable(testrunner testrunner.cpp "${src_tests}")

if(${ENABLE_GOOGLETEST})
    target_link_libraries(testrunner gtest devfixnet devfixdsp pthread)
endif()
