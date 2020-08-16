//
// Created by core on 6/2/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <testutil.h>
#include <array>
#include "../strcvt.h"

using namespace devfix::base;

/*
 * Test A:
 * [A,B,U+12345,lambda,8]
 */
constexpr std::array test_a_utf08{ 0x41, 0x42, 0xF0, 0x92, 0x8D, 0x85, 0xCE, 0xBB, 0x38 };
constexpr std::array test_a_utf16{ 0x41, 0x00, 0x42, 0x00, 0x08, 0xD8, 0x45, 0xDF, 0xBB, 0x03, 0x38, 0x00 };
constexpr std::array test_a_utf32
	{ 0x41, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x45, 0x23, 0x01, 0x00, 0xBB, 0x03, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00 };

template<typename T, std::size_t N>
std::basic_string<T> uXXs(const std::array<int, N>& arr)
{
	std::array<char, N> raw{};
	for (std::size_t i = 0; i < N; i++) { raw.at(i) = static_cast<char>(arr.at(i)); }
	std::basic_string<T> s(N / sizeof(T), static_cast<T>(0));
	for (std::size_t i = 0; i < N / sizeof(T); i++) { s.at(i) = static_cast<T>(reinterpret_cast<const T*>(raw.data())[i]); }
	return s;
}

template<std::size_t N>
auto u08s(const std::array<int, N>& arr) { return uXXs<char08_t, N>(arr); }

template<std::size_t N>
auto u16s(const std::array<int, N>& arr) { return uXXs<char16_t, N>(arr); }

template<std::size_t N>
auto u32s(const std::array<int, N>& arr) { return uXXs<char32_t, N>(arr); }

const auto a08 = u08s(test_a_utf08);
const auto a16 = u16s(test_a_utf16);
const auto a32 = u32s(test_a_utf32);

TEST_CASE("StrCvt - u08 <> u16")
{
	CHECK(strcvt::c08to16(a08) == a16);
	CHECK(strcvt::c16to08(a16) == a08);
}

TEST_CASE("StrCvt - u16 <> u32")
{
	CHECK(strcvt::c16to32(a16) == a32);
	CHECK(strcvt::c32to16(a32) == a16);
}

TEST_CASE("StrCvt - u08 <> u32")
{
	CHECK(strcvt::c08to32(a08) == a32);
	CHECK(strcvt::c32to08(a32) == a08);
}

#endif
