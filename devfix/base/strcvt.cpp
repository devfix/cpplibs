//
// Created by core on 7/7/20.
//

#include <locale>
#include <codecvt>
#include "strcvt.h"

namespace devfix::base
{

	std::wstring strcvt::to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	std::string strcvt::to_string(const std::wstring& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(str);
	}

}
