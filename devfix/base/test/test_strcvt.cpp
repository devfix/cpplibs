//
// Created by core on 6/2/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch2/catch.hpp>
#include <array>
#include "../strcvt.h"

using namespace devfix::base;

constexpr double PRECISION = 1e-6;

const std::array<unsigned char, 3> test_a_utf8{ 0xC3, 0xB6, 0x00 }; // test german oe
const std::array<unsigned char, 4> test_a_utf16{ 0xF6, 0x00, 0x00, 0x00 };
const std::array<unsigned char, 5> test_b_utf8{ 0xC3, 0xB6, 0xCE, 0xBB, 0x00 };  // german oe and lambda
const std::array<unsigned char, 6> test_b_utf16{ 0xF6, 0x00, 0xBB, 0x03, 0x00, 0x00 };

std::string a2sv(const char* arr) { return std::string(arr); }
std::string a2sv(const unsigned char* arr) { return a2sv(reinterpret_cast<const char*>(arr)); }

std::wstring a2wsv(const wchar_t* arr) { return std::wstring(arr); }
std::wstring a2wsv(const unsigned char* arr) { return a2wsv(reinterpret_cast<const wchar_t*>(arr)); }

TEST_CASE("StrCvt - to_wstring")
{
	CHECK(strcvt::wstr(a2sv("")).empty());
	CHECK(strcvt::wstr(a2sv("test")) == L"test");
	CHECK(strcvt::wstr(a2sv("test\n")) == L"test\n");
	CHECK(strcvt::wstr(a2sv(test_a_utf8.data())) == std::wstring(a2wsv(test_a_utf16.data())));
	CHECK(strcvt::wstr(a2sv(test_b_utf8.data())) == std::wstring(a2wsv(test_b_utf16.data())));
}

TEST_CASE("StrCvt - to_string")
{
	CHECK(strcvt::str(a2wsv(L"")).empty());
	CHECK(strcvt::str(a2wsv(L"test")) == "test");
	CHECK(strcvt::str(a2wsv(L"test\n")) == "test\n");
	CHECK(
		strcvt::str(a2wsv(test_a_utf16.data()))
			== std::string(a2sv(test_a_utf8.data())));
	CHECK(strcvt::str(a2wsv(test_b_utf16.data()))
			  == std::string(a2sv(test_b_utf8.data())));
}

TEST_CASE("StrCvt - stof")
{
	CHECK(strcvt::stof(std::string("+0.5")) == Approx(0.5f).epsilon(PRECISION));
	CHECK(strcvt::stof(std::string("1.9")) == Approx(1.9f).epsilon(PRECISION));
	CHECK(strcvt::stof(std::string("-6")) == Approx(-6.f).epsilon(PRECISION));
	CHECK(strcvt::stof(std::string("-0.1")) == Approx(-0.1f).epsilon(PRECISION));
}

TEST_CASE("StrCvt - stod")
{
	CHECK(strcvt::stod(std::string("+0.5")) == Approx(0.5).epsilon(PRECISION));
	CHECK(strcvt::stod(std::string("1.9")) == Approx(1.9).epsilon(PRECISION));
	CHECK(strcvt::stod(std::string("-6")) == Approx(-6.).epsilon(PRECISION));
	CHECK(strcvt::stod(std::string("-0.1")) == Approx(-0.1).epsilon(PRECISION));
}

TEST_CASE("StrCvt - stold")
{
	CHECK(strcvt::stold(std::string("+0.5")) == Approx(0.5l).epsilon(PRECISION));
	CHECK(strcvt::stold(std::string("1.9")) == Approx(1.9l).epsilon(PRECISION));
	CHECK(strcvt::stold(std::string("-6")) == Approx(-6.l).epsilon(PRECISION));
	CHECK(strcvt::stold(std::string("-0.1")) == Approx(-0.1l).epsilon(PRECISION));
}

#endif