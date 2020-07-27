//
// Created by core on 7/7/20.
//

#include <codecvt>
#include "strcvt.h"

namespace devfix::base
{

	std::wstring strcvt::wstr(std::string_view sv) { return wstr(std::string(sv)); }

	std::wstring strcvt::wstr(const std::string& str)
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(str);
	}

	std::string strcvt::str(std::wstring_view wsv) { return str(std::wstring(wsv)); }

	std::string strcvt::str(const std::wstring& wstr)
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(wstr);
	}

}
