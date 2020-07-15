//
// Created by core on 7/7/20.
//
#pragma once

#include <string>

namespace devfix::base
{
	struct strcvt
	{
		[[nodiscard]] static std::wstring wstr(const std::string& str);
		[[nodiscard]] static std::string str(const std::wstring& str);
	};
}
