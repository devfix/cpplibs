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
    #target_link_libraries(testrunner gtest devfixnet devfixdsp pthread)
endif()


########
set(gtest_disable_pthreads ON)
set(CMAKE_CXX_EXTENSIONS ON)
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(testrunner gtest devfixnet devfixdsp pthread)
