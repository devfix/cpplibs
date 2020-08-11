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

			// remove spaces
			for (std::size_t pos = 0; (pos = retval.find_first_of(' ', pos)) != std::string::npos;) { retval.erase(pos, 1); }
			return retval;
		}
		throw std::invalid_argument("cannot demangle type name");
	}

#else
	std::string type::demangle(const char* mangled_name)
	{
		std::string name(mangled_name);

		constexpr std::string_view mvc_prefix_class = "class ";
		constexpr std::string_view mvc_prefix_struct= "struct ";
		for (std::size_t pos = 0; (pos = name.find(mvc_prefix_class, pos)) != std::string::npos;) { name.erase(pos, mvc_prefix_class.length()); }
		for (std::size_t pos = 0; (pos = name.find(mvc_prefix_struct, pos)) != std::string::npos;) { name.erase(pos, mvc_prefix_struct.length()); }
		
		// remove spaces
		for (std::size_t pos = 0; (pos = name.find_first_of(' ', pos)) != std::string::npos;) { name.erase(pos, 1); }

		// remove suffix of pointers (mvc)
		if (std::size_t pos = name.find_first_of('*'); pos != std::string::npos && pos != name.length() - 1)
		{
			name.erase(pos + 1, name.length() - pos - 1);
		}
		return name;
	}
#endif

	std::string type::remove_nested(const std::string& name)
	{
#if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION
		static const std::regex re("^std::__nested<(.*)>$");
		std::smatch match;
		if (std::regex_match(name, match, re)) { return match[1].str(); }
#elif defined(__GLIBCXX__) && __GLIBCXX__
		static const std::regex re("^std::_Nested_exception<(.*)>$");
		std::smatch match;
		if (std::regex_match(name, match, re)) { return match[1].str(); }
#else
		static const std::regex re("^std::_With_nested<(.*),(.*)>$");
		std::smatch match;
		if (std::regex_match(name, match, re)) { return match[1].str(); }
#endif
		return name;
	}

}


