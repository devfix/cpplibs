//
// Created by core on 7/7/20.
//
#pragma once

#include <string>
#include <locale>

namespace devfix::base
{
	struct strcvt
	{
		/**
		 * \brief convert std::string to std::wstring
		 * \param sv input string
		 * \return converted wstring
		 */
		[[nodiscard]] static std::wstring wstr(std::string_view sv);

		/**
		 * \brief convert std::string to std::wstring
		 * \param str input string
		 * \return converted wstring
		 */
		[[nodiscard]] static std::wstring wstr(const std::string& str);

		/**
		 * \brief convert std::wstring to std::string
		 * \param wsv input wstring
		 * \return converted string
		 */
		[[nodiscard]] static std::string str(std::wstring_view wsv);

		/**
		 * \brief convert std::wstring to std::string
		 * \param wstr input wstring
		 * \return converted string
		 */
		[[nodiscard]] static std::string str(const std::wstring& wstr);

		/**
		 * \brief convert string to float always with "C" locale
		 * \tparam CharT character type, either 'char' or 'wchar_t'
		 * \param str input string
		 * \return converted float
		 */
		template<typename CharT>
		[[nodiscard]] static auto stof(const std::basic_string<CharT>& str)
		{
			const auto old_locale = std::setlocale(LC_NUMERIC, nullptr);
			std::setlocale(LC_NUMERIC, "C");
			auto d = std::stof(str);
			std::setlocale(LC_NUMERIC, old_locale);
			return d;
		}

		/**
		 * \brief convert string to float always with "C" locale
		 * \tparam CharT character type, either 'char' or 'wchar_t'
		 * \param sv input string
		 * \return converted float
		 */
		template<typename CharT>
		[[nodiscard]] static auto stof(std::basic_string_view<CharT> sv)
		{
			return std::basic_string<CharT>(stof(sv));
		}

		/**
		 * \brief convert string to double always with "C" locale
		 * \tparam CharT character type, either 'char' or 'wchar_t'
		 * \param str input string
		 * \return converted double
		 */
		template<typename CharT>
		[[nodiscard]] static auto stod(const std::basic_string<CharT>& str)
		{
			const auto old_locale = std::setlocale(LC_NUMERIC, nullptr);
			std::setlocale(LC_NUMERIC, "C");
			auto d = std::stod(str);
			std::setlocale(LC_NUMERIC, old_locale);
			return d;
		}

		/**
		 * \brief convert string to double always with "C" locale
		 * \tparam CharT character type, either 'char' or 'wchar_t'
		 * \param sv input string
		 * \return converted double
		 */
		template<typename CharT>
		[[nodiscard]] static auto stod(std::basic_string_view<CharT> sv)
		{
			return std::basic_string<CharT>(stod(sv));
		}

		/**
		 * \brief convert string to long double always with "C" locale
		 * \tparam CharT character type, either 'char' or 'wchar_t'
		 * \param str input string
		 * \return converted long double
		 */
		template<typename CharT>
		[[nodiscard]] static auto stold(const std::basic_string<CharT>& str)
		{
			const auto old_locale = std::setlocale(LC_NUMERIC, nullptr);
			std::setlocale(LC_NUMERIC, "C");
			auto d = std::stold(str);
			std::setlocale(LC_NUMERIC, old_locale);
			return d;
		}

		/**
		 * \brief convert string to long double always with "C" locale
		 * \tparam CharT character type, either 'char' or 'wchar_t'
		 * \param sv input string
		 * \return converted long double
		 */
		template<typename CharT>
		[[nodiscard]] static auto stold(std::basic_string_view<CharT> sv)
		{
			return std::basic_string<CharT>(stold(sv));
		}

	};
}
