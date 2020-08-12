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

	private:
		static void remove_number_suffix(std::string& name);
		static void remove_type_label(std::string& name);
		static void remove_spaces(std::string& name);
		static void remove_pointer_suffix(std::string& name);
	};
}
