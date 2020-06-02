//
// Created by core on 12/19/19.
//

#pragma once

// set linux specific definitions
#ifdef __linux__
#define PLATFORM_LINUX 1
#else
#define PLATFORM_LINUX 0
#endif

#ifdef _WIN32
#define PLATFORM_WINDOWS 1
#else
#define PLATFORM_WINDOWS 0
#endif

#define ERROR_PLATFORM_UNSUPPORTED static_assert ( false, "Platform not supported" )

// __FILENAME__ gets supplied via compiler options
// check if cmake is configured properly
#ifndef __FILENAME__
#pragma "__FILENAME__ not defined, build script broken? Falling back to __FILE__"
#define __FILENAME__ &__FILE__[0]
#endif

// macro to get current source line and attributes
#define SOURCE_LINE std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + ": in \"" + std::string(&__FUNCTION__[0]) + "\""

// macro to mark return values statements as unused
#define NOT_USED(x) static_cast<void>(x)
