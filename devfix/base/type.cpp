//
// Created by core on 7/9/20.
//

#include <stdexcept>
#include <regex>

#ifdef __GNUG__
#include <cxxabi.h>

#endif

#include "type.h"
#include "strutil.h"

namespace devfix::base
{

	std::string type::demangle(const char* mangled_name)
	{
		std::string name(mangled_name);

#ifdef __GNUG__
		// gnu demangle
		int status;
		char* cxa_name = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
		if (status == 0 && cxa_name != nullptr)
		{
			name = cxa_name;
			free(cxa_name);
		}
		else { throw std::invalid_argument("cannot demangle type name"); }
#endif

		remove_type_label(name);
		remove_spaces(name);
		remove_number_suffix(name);
		remove_pointer_suffix(name);

		return name;
	}

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

	void type::remove_number_suffix(std::string& name)
	{
		static const std::string_view regex = "(,|<|^)((\\.)|([0-9]))+(u|l|U|L)";
		for (auto res = strutil::find_regex(name, std::string(regex)); res.has_value(); res = strutil::find_regex(name, std::string(regex)))
		{
			name.erase(res->first + res->second - 1, 1);
		}
	}

	void type::remove_type_label(std::string& name)
	{
		static const std::string_view regex = "((class)|(struct)|(enum)) ";
		for (auto res = strutil::find_regex(name, std::string(regex)); res.has_value(); res = strutil::find_regex(name, std::string(regex)))
		{
			name.erase(res->first, res->second);
		}
	}

	void type::remove_spaces(std::string& name)
	{
		for (std::size_t pos = 0; (pos = name.find_first_of(' ', pos)) != std::string::npos;)
		{
			name.erase(pos, 1);
		}
	}

	void type::remove_pointer_suffix(std::string& name)
	{
		static const std::string_view regex = "\\*[A-Za-z0-9_]+(\\*|,|>|$)";
		for (auto res = strutil::find_regex(name, std::string(regex)); res.has_value(); res = strutil::find_regex(name, std::string(regex)))
		{
			const std::size_t pos = res->first + 1;
			std::size_t len = res->second - 1;
			if (pos + len != name.length()) { len--; }
			name.erase(pos, len);
		}
	}

}

