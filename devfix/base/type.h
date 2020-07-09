//
// Created by core on 7/9/20.
//

#pragma once

#include <string>

namespace devfix::base
{
	struct type
	{
		type() = delete;

		[[nodiscard]] static std::string demangle(const char* mangled_name);
		[[nodiscard]] static std::string remove_nested(const std::string& name);
	};
}
