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

	std::string type::demangle(const char* mangled_name)
	{

#ifdef __GNUG__
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
#else
		std::string name(mangled_name);

		// remove class, struct, enum
		{
			static const std::regex re("((class)|(struct)|(enum)) ");
			std::smatch match;
			while (std::regex_search(name, match, re)) {
				name = std::string(match.prefix()) + std::string(match.suffix());
			}
		}
		
		// remove spaces
		for (std::size_t pos = 0; (pos = name.find_first_of(' ', pos)) != std::string::npos;) { name.erase(pos, 1); }

		// remove pointer suffixes
		static const std::regex re("\\*[A-Za-z0-9_]+(\\*|,|>|$)");
		std::smatch match;
		while (std::regex_search(name, match, re)) {
			const auto pre = std::string(match.prefix());
			const bool end = match.suffix().length() == 0;
			name = pre + "*" + name.substr(pre.length() + match[0].length() - (end ? 0 : 1));
		}

		return name;
#endif
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

}


