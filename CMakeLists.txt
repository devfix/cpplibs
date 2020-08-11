################################
# ~~~ cpplibs ~~~
#
# optional cmake arguments:
# -DCPPLIBS_ENABLE_TESTS=0
# -DCMAKE_VERBOSE_MAKEFILE=true
################################

cmake_minimum_required(VERSION 3.0)


find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
else ()
    message(STATUS "ccache not found")
endif ()


project(cpplibs CXX)



#######################
# detect enable tests #
#######################
set(CMAKE_CXX_STANDARD 20)
if (NOT DEFINED CPPLIBS_ENABLE_TESTS)
    set(CPPLIBS_ENABLE_TESTS 1)  # tests are enabled by default
endif ()
if (${CPPLIBS_ENABLE_TESTS})
    if (NOT DEFINED CPPLIBS_ENABLE_DSP_TESTS)
        set(CPPLIBS_ENABLE_DSP_TESTS 1)  # tests are enabled by default
    endif ()
    if (NOT DEFINED CPPLIBS_ENABLE_NET_TESTS)
        set(CPPLIBS_ENABLE_NET_TESTS 1)  # tests are enabled by default
    endif ()
endif ()

if(NOT ${CPPLIBS_ENABLE_TESTS})
    message(STATUS "cpplibs tests are disabled")
else()
    if(NOT ${CPPLIBS_ENABLE_DSP_TESTS})
        message(STATUS "cpplibs dsp tests are disabled")
    endif()
    if(NOT ${CPPLIBS_ENABLE_NET_TESTS})
        message(STATUS "cpplibs net tests are disabled")
    endif()
endif()



###########################
# compiler specific flags #
###########################

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCPPLIBS_ENABLE_TESTS=${CPPLIBS_ENABLE_TESTS} -DCPPLIBS_ENABLE_DSP_TESTS=${CPPLIBS_ENABLE_DSP_TESTS} -DCPPLIBS_ENABLE_NET_TESTS=${CPPLIBS_ENABLE_NET_TESTS}")

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
endif(UNIX)

if (WIN32)
    option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DCPPLIBS_ENABLE_TESTS=${CPPLIBS_ENABLE_TESTS} /DCPPLIBS_ENABLE_DSP_TESTS=${CPPLIBS_ENABLE_DSP_TESTS} /DCPPLIBS_ENABLE_NET_TESTS=${CPPLIBS_ENABLE_NET_TESTS}")
endif(WIN32)



############################
# cpplibs static libraries #
############################

add_subdirectory(devfix/base)
add_subdirectory(devfix/net)
add_subdirectory(devfix/dsp)



#####################
# testrunner target #
#####################

if (${CPPLIBS_ENABLE_TESTS})
    file(GLOB_RECURSE src_tests "${CMAKE_CURRENT_LIST_DIR}/devfix/*test_*.cpp")
    add_executable(testrunner testrunner.cpp "${src_tests}")
    target_include_directories(testrunner PUBLIC "Catch2/single_include")  # include Catch2 library header files
    target_link_libraries(testrunner devfixnet devfixdsp)

    if (UNIX)
        target_link_libraries(testrunner pthread)
    endif()
endif ()
