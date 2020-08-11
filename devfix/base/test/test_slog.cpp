//
// Created by core on 7/7/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch2/catch.hpp>
#include <iostream>
#include "../slog.h"

using namespace devfix::base;

TEST_CASE("slog - simple char")
{
	std::stringstream out;
	slog<char> sl(&out);
	sl.set_prefix("[abc] ");

	sl << "test";
	CHECK(out.str() == "[abc] test");

	out.str("");
	sl << "test\n";
	CHECK(out.str() == "[abc] test\n");

	out.str("");
	sl << "test\n123";
	CHECK(out.str() == "[abc] test\n[abc] 123");

	out.str("");
	sl << "test\n123\n";
	CHECK(out.str() == "[abc] test\n[abc] 123\n");
}

TEST_CASE("slog - simple wchar")
{
	std::wstringstream out;
	slog<wchar_t > sl(&out);
	sl.set_prefix(L"[abc] ");

	sl << L"test";
	CHECK(out.str() == L"[abc] test");

	out.str(L"");
	sl << L"test\n";
	CHECK(out.str() == L"[abc] test\n");

	out.str(L"");
	sl << L"test\n123";
	CHECK(out.str() == L"[abc] test\n[abc] 123");

	out.str(L"");
	sl << L"test\n123\n";
	CHECK(out.str() == L"[abc] test\n[abc] 123\n");
}

TEST_CASE("slog - stx char")
{
	std::stringstream out;
	slog<char> sl(&out);
	sl.set_prefix("[abc] ");

	sl << "test" << stx;
    std::cout << "UFFFFF\"" << out.str() << "\"\n";
	CHECK(out.str() == "[abc] test\033[2K\r");

	out.str("");
	sl << "test\n" << stx << "test";
	CHECK(out.str() == "[abc] test\n\033[2K\r[abc] test");

	out.str("");
	sl << "test\n" << stx << "test\n" << etx;
	CHECK(out.str() == "[abc] test\n\033[2K\r[abc] test\n");
}

TEST_CASE("slog - stx wchar")
{
	std::wstringstream out;
	slog<wchar_t> sl(&out);
	sl.set_prefix(L"[abc] ");

	sl << L"test" << stx;
	CHECK(out.str() == L"[abc] test\033[2K\r");

	out.str(L"");
	sl << L"test\n" << stx << L"test";
	CHECK(out.str() == L"[abc] test\n\033[2K\r[abc] test");

	out.str(L"");
	sl << L"test\n" << stx << L"test\n" << etx;
	CHECK(out.str() == L"[abc] test\n\033[2K\r[abc] test\n");
}

TEST_CASE("slog - cascade")
{
	std::stringstream out;
	slog<char> sl1(&out);
	sl1.set_prefix("[1]");
	slog<char> sl2(&sl1);
	sl2.set_prefix("[2]");
	slog<char> sl3(&sl2);
	sl3.set_prefix("[3]");

	sl3 << "test";
	CHECK(out.str() == "[1][2][3]test");

	out.str("");
	sl3 << "test\n";
	CHECK(out.str() == "[1][2][3]test\n");

	out.str("");
	sl3 << "test\n123";
	CHECK(out.str() == "[1][2][3]test\n[1][2][3]123");

	out.str("");
	sl3 << "test\n123\n";
	CHECK(out.str() == "[1][2][3]test\n[1][2][3]123\n");

	out.str("");
	sl3 << "test" << stx << "abc";
	CHECK(out.str() == "[1][2][3]test\033[2K\r[1][2][3]abc");
}

#endif
