cmake_minimum_required(VERSION 3.0)
project(cpplibs CXX)

set(CMAKE_CXX_STANDARD 20)
set(ENABLE_GOOGLETEST 1)

find_package(Doxygen)

set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS} -D ENABLE_GOOGLETEST=${ENABLE_GOOGLETEST}")

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

    add_custom_target(documentation
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )

endif(DOXYGEN_FOUND)


add_executable(testrunner devfix/net/test/test_socket.cpp testrunner.cpp devfix/net/test/test_inetaddress.cpp devfix/dsp/test/test_window.cpp devfix/dsp/test/test_fft_old.cpp devfix/dsp/test/rosettafft.h devfix/dsp/test/rosettafft.cpp devfix/base/test/test_math.cpp devfix/dsp/test/test_addr.cpp devfix/dsp/test/test_fft.cpp)
if(${ENABLE_GOOGLETEST})
    target_link_libraries(testrunner gtest devfixnet devfixdsp pthread)
endif()

# test stuff during sinus
target_link_libraries(testrunner devfixdsp)
