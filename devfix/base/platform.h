//
// Created by core on 12/19/19.
//

#pragma once

#ifdef __linux__
#define PLATFORM_LINUX 1
#else
#define PLATFORM_LINUX 0
#endif

#define PLATFORM_UNSUPPORTED static_assert ( false, "Platform not supported" )

#ifndef __FILENAME__
#pragma "__FILENAME__ not defined, build script broken? Falling back to __FILE__"
#define __FILENAME__ &__FILE__[0]
#endif

// makro to get current source line and attributes
#define SOURCE_LINE std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + ": in \"" + std::string(&__FUNCTION__[0]) + "\""
