//
// Created by core on 06.05.2020.
//

#pragma once

#include <string>


#define MULTISTRING(CharT, str) devfix::base::get_from_multistring<CharT>(str, L##str)


namespace devfix::base
{
	template<typename T>
	constexpr const T* get_from_multistring(const char* str, const wchar_t* wstr) {}

	template<>
	constexpr const char* get_from_multistring<char>(const char* str, const wchar_t* wstr) { return str; }

	template<>
	constexpr const wchar_t* get_from_multistring<wchar_t>(const char* str, const wchar_t* wstr) { return wstr; }

	struct strutil
	{

	};

}