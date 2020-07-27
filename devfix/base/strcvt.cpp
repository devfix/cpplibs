//
// Created by core on 7/7/20.
//

#include <codecvt>
#include "strcvt.h"

namespace devfix::base
{

	std::wstring strcvt::wstr(std::string_view sv)
	{
		std::string str(sv);
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(str);
	}

	std::string strcvt::str(std::wstring_view wsv)
	{
		std::wstring wstr(wsv);
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(wstr);
	}

}
