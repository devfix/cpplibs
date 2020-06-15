//
// Created by core on 12/19/19.
//

#pragma once

namespace devfix::base
{

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


// macro to mark return values statements as unused
#define NOT_USED(x) static_cast<void>(x)

struct platform
{
	static constexpr const char * get_filename(const char *fname)
	{
		const char* f = nullptr;
		const char* p = fname;
		for(; *p; p++)
		{
			if(*p == '/' || *p == '\\')
			{ f = ++p; }
		}
		return f ? f : fname;
	}
};

// macro to get current source line and attributes
#ifdef NDEBUG
#define SOURCE_LINE std::string(devfix::base::platform::get_filename(__FILE__)) + ":" + std::to_string(__LINE__) + ": in \"" + &__FUNCTION__[0] + "\""
#else
#define SOURCE_LINE std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": in \"" + &__FUNCTION__[0] + "\""
#endif

}