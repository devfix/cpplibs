//
// Created by core on 7/7/20.
//
#pragma once

#include <string>

namespace devfix::base
{
	struct strcvt
	{
		[[nodiscard]] static std::wstring to_wstring(const std::string& str);
		[[nodiscard]] static std::string to_string(const std::wstring& str);
	};
}
