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

// makro to get current source line and attributes
#define SOURCE_LINE std::string(&__FILE__[0]) + ":" + std::to_string(__LINE__) + ": in \"" + std::string(&__FUNCTION__[0]) + "\""
