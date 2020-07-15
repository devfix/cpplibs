//
// Created by core on 6/2/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch/catch.hpp>
#include "../strcvt.h"

using namespace devfix::base;

const std::array<unsigned char, 3> test_a_utf8{ 0xC3, 0xB6, 0x00 }; // test german oe
const std::array<unsigned char, 8> test_a_utf16{ 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const std::array<unsigned char, 5> test_b_utf8{ 0xC3, 0xB6, 0xCE, 0xBB, 0x00 };  // german oe and lambda
const std::array<unsigned char, 12> test_b_utf16{ 0xF6, 0x00, 0x00, 0x00, 0xBB, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

TEST_CASE("StrCvt - to_wstring")
{
	CHECK(strcvt::wstr("").empty());
	CHECK(strcvt::wstr("test") == L"test");
	CHECK(strcvt::wstr("test\n") == L"test\n");
	CHECK(strcvt::wstr(reinterpret_cast<const char*>(test_a_utf8.data()))
			  == std::wstring(reinterpret_cast<const wchar_t*>(test_a_utf16.data())));
	CHECK(strcvt::wstr(reinterpret_cast<const char*>(test_b_utf8.data()))
			  == std::wstring(reinterpret_cast<const wchar_t*>(test_b_utf16.data())));
}

TEST_CASE("StrCvt - to_string")
{
	CHECK(strcvt::str(L"").empty());
	CHECK(strcvt::str(L"test") == "test");
	CHECK(strcvt::str(L"test\n") == "test\n");
	CHECK(
		strcvt::str(reinterpret_cast<const wchar_t*>(test_a_utf16.data()))
			== std::string(reinterpret_cast<const char*>(test_a_utf8.data())));
	CHECK(strcvt::str(reinterpret_cast<const wchar_t*>(test_b_utf16.data()))
			  == std::string(reinterpret_cast<const char*>(test_b_utf8.data())));
}

#endif