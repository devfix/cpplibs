//
// Created by core on 06.05.2020.
//

#pragma once

namespace devfix::base
{

	struct StrUtil
	{
	private:
		template<class Facet>
		struct deletable_facet : Facet
		{
			template<class ...Args>
			deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...)
			{}
			~deletable_facet()
			{}
		};

	public:
		template<typename T>
		static std::string encode(const T* wstr)
		{
			return std::wstring_convert<deletable_facet<std::codecvt<T, char, std::mbstate_t>>, T>().to_bytes(wstr);
		}

		template<typename T>
		static std::string encode(const std::basic_string<T>& wstr)
		{
			return encode<T>(wstr.data());
		}

		template<typename T>
		static std::basic_string<T> decode(const char* str)
		{
			return std::wstring_convert<deletable_facet<std::codecvt<T, char, std::mbstate_t>>, T>().from_bytes(str);
		}

		template<typename T>
		static std::basic_string<T> decode(const std::string& str)
		{
			return decode<T>(str.data());
		}

	};

}