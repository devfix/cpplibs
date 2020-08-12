//
// Created by core on 06.05.2020.
//

#pragma once

#include <string>
#include <tuple>
#include <optional>
#include <regex>


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
		template<typename CharT>
		static std::optional<std::pair<std::size_t, std::size_t>> find_regex(const std::basic_string<CharT>& sv,
			const std::basic_string<CharT>& regex)
		{
			const std::basic_regex<CharT> re(regex);
			std::smatch match;
			if (std::regex_search(sv, match, re)) { return {{ match.position(0), match[0].length() }}; }
			return std::nullopt;
		}
	};

}