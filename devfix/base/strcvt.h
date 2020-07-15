//
// Created by core on 7/7/20.
//
#pragma once

#include <string>

namespace devfix::base
{
	struct strcvt
	{
		/**
		 * \brief convert std::string to std::wstring
		 * \param str input string
		 * \return converted wstring
		 */
		[[nodiscard]] static std::wstring wstr(const std::string& str);

		/**
		 * \brief convert std::wstring to std::string
		 * \param wstr input wstring
		 * \return converted string
		 */
		[[nodiscard]] static std::string str(const std::wstring& wstr);
	};
}
