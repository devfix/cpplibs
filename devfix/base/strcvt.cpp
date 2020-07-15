//
// Created by core on 7/7/20.
//

#include <locale>
#include <codecvt>
#include "strcvt.h"

namespace devfix::base
{

	std::wstring strcvt::wstr(const std::string& str)
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(str);
	}

	std::string strcvt::str(const std::wstring& wstr)
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(wstr);
	}

}
