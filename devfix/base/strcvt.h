//
// Created by core on 7/7/20.
//
#pragma once

#include <string>
#include <cstdint>
#include <locale>
#include <algorithm>

//#ifdef __cplusplus > 201703L
//using char08_t = char8_t;
//#else
using char08_t = char;
//#endif

namespace devfix::base
{
	struct strcvt
	{
		using u08string= std::basic_string<char08_t>;
		using u16string = std::u16string;
		using u32string = std::u32string;

		[[nodiscard]] static u16string c08to16(const u08string& s);
		[[nodiscard]] static u32string c08to32(const u08string& s);
		[[nodiscard]] static u08string c16to08(const u16string& s);
		[[nodiscard]] static u32string c16to32(const u16string& s);
		[[nodiscard]] static u08string c32to08(const u32string& s);
		[[nodiscard]] static u16string c32to16(const u32string& s);


	private:
		template<typename T, std::size_t W = sizeof(T)>
		struct wstr_t;
		template<typename T>
		struct wstr_t<T, 2>
		{
			std::wstring conv(const std::string& str)
			{
				u08string u8(str.length(), 0);
				std::transform(str.begin(), str.end(), u8.begin(), [](auto c) { return c; });
				auto u16 = c08to16(u8);
				std::wstring wstr(u16.length(), 0);
				std::transform(u16.begin(), u16.end(), wstr.begin(), [](auto c) { return c; });
				return wstr;
			}
		};
		template<typename T>
		struct wstr_t<T, 4>
		{
			std::wstring conv(const std::string& str)
			{
				u08string u8(str.length(), 0);
				std::transform(str.begin(), str.end(), u8.begin(), [](auto c) { return c; });
				auto u32 = c08to32(u8);
				std::wstring wstr(u32.length(), 0);
				std::transform(u32.begin(), u32.end(), wstr.begin(), [](auto c) { return c; });
				return wstr;
			}
		};

		template<typename T, std::size_t W = sizeof(T)>
		struct str_t;
		template<typename T>
		struct str_t<T, 2>
		{
			std::string conv(const std::wstring& wstr)
			{
				u16string u16(wstr.length(), 0);
				std::transform(wstr.begin(), wstr.end(), u16.begin(), [](auto c) { return c; });
				auto u8 = c16to08(u16);
				std::string str(u8.length(), 0);
				std::transform(u8.begin(), u8.end(), str.begin(), [](auto c) { return c; });
			}
		};
		template<typename T>
		struct str_t<T, 4>
		{
			std::string conv(const std::wstring& wstr)
			{
				u32string u32(wstr.length(), 0);
				std::transform(wstr.begin(), wstr.end(), u32.begin(), [](auto c) { return c; });
				auto u8 = c32to08(u16);
				std::string str(u8.length(), 0);
				std::transform(u8.begin(), u8.end(), str.begin(), [](auto c) { return c; });
			}
		};

	public:
		/**
		 * \brief convert std::string to std::wstring
		 * \param str input string
		 * \return converted wstring
		 */
		[[nodiscard]] std::wstring wstr(const std::string& str) { return wstr_t<wchar_t>().conv(str); }

		/**
		 * \brief convert std::wstring to std::string
		 * \param wstr input wstring
		 * \return converted string
		 */
		[[nodiscard]] std::string str(const std::wstring& wstr) { return str_t<wchar_t>().conv(wstr); }

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
			return stof(std::basic_string<CharT>(sv));
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
			return stod(std::basic_string<CharT>(sv));
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
			return stold(std::basic_string<CharT>(sv));
		}

	};
}
