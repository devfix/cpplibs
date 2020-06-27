cmake_minimum_required(VERSION 3.0)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
else()
    message(WARNING "ccache not found")
endif()

project(cpplibs CXX)

set(CMAKE_CXX_STANDARD 20)
set(ENABLE_GOOGLETEST 1)
set(CMAKE_VERBOSE_MAKEFILE false)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D ENABLE_GOOGLETEST=${ENABLE_GOOGLETEST}")

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

file(GLOB_RECURSE src_tests ${CMAKE_CURRENT_SOURCE_DIR} "test_*.cpp")
add_executable(testrunner testrunner.cpp "${src_tests}")

if(${ENABLE_GOOGLETEST})
    target_link_libraries(testrunner gtest devfixnet devfixdsp pthread)
endif()
