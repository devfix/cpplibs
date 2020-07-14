//
// Created by core on 7/9/20.
//

#include <stdexcept>
#include <regex>

#ifdef __GNUG__
#include <cxxabi.h>

#endif

#include "type.h"

namespace devfix::base
{

#ifdef __GNUG__

	std::string type::demangle(const char* mangled_name)
	{
		int status;
		char* name = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
		if (status == 0 && name != nullptr)
		{
			std::string retval(name);
			free(name);
			return retval;
		}
		throw std::invalid_argument("cannot demangle type name");
	}

#else
	std::string type::demangle(const char* mangled_name) { return mangled_name; }
#endif

	std::string type::remove_nested(const std::string& name)
	{
#if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION
		static const std::regex re("^std::__nested<(.*)>$");
#elif defined(__GLIBCXX__) && __GLIBCXX__
		static const std::regex re("^std::_Nested_exception<(.*)>$");
#endif
		std::smatch match;
		if (std::regex_match(name, match, re)) { return match[1].str(); }
		return name;
	}

}


