//
// Created by core on 5/25/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <testutil.h>
#include "../math.h"

using namespace devfix::base;

TEST_CASE("Math - countl_zero")
{
	CHECK(math::countl_zero(std::uint8_t(0b00000000)) == 8);
	CHECK(math::countl_zero(std::uint8_t(0b00101010)) == 2);
	CHECK(math::countl_zero(std::uint8_t(0b11111111)) == 0);
	CHECK(math::countl_zero(std::uint8_t(0b00000111)) == 5);
	CHECK(math::countl_zero(std::uint64_t(1)) == 63);
	CHECK(math::countl_zero(std::uint64_t(0)) == 64);
	CHECK(math::countl_zero(std::uint64_t(-1)) == 0);
}

TEST_CASE("Math - floorLog2")
{
	CHECK(math::floorLog2(std::uint8_t(0b01001000)) == 6);
	CHECK(math::floorLog2(std::uint8_t(0b01001000)) == 6);
	CHECK(math::floorLog2(std::uint8_t(0b00010000)) == 4);
	CHECK(math::floorLog2(std::uint8_t(0b00000001)) == 0);
	CHECK(math::floorLog2(std::uint64_t(10000)) == 13);
	CHECK(math::floorLog2(std::uint64_t(100000)) == 16);
	CHECK_THROWS(math::floorLog2(0u));
}

TEST_CASE("Math - Log2")
{
	CHECK(math::popcount(1u) == 0);
	CHECK(math::popcount(2u) == 1);
	CHECK(math::popcount(4u) == 2);
	CHECK(math::popcount(8u) == 3);
	CHECK(math::popcount(16u) == 4);
	CHECK(math::popcount(32u) == 5);
	CHECK(math::popcount(64u) == 6);
	CHECK(math::popcount(128u) == 7);
}

TEST_CASE("Math - mod")
{
	CHECK(math::mod(3, 4) == 3);
	CHECK(math::mod(7, 4) == 3);
	CHECK(math::mod(11, 9) == 2);
	CHECK(math::mod(-1, 2) == 1);
	CHECK(math::mod(-6, 3) == 0);
	CHECK(math::mod(-7, 3) == 2);

	CHECK(math::mod(3., 4.) == Approx(3.).margin(testutil::MARGIN_FINE));
	CHECK(math::mod(7., 4.) == Approx(3.).margin(testutil::MARGIN_FINE));
	CHECK(math::mod(11., 9.) == Approx(2.).margin(testutil::MARGIN_FINE));
	CHECK(math::mod(-1., 2.) == Approx(1.).margin(testutil::MARGIN_FINE));
	CHECK(math::mod(-6., 3.) == Approx(0.).margin(testutil::MARGIN_FINE));
	CHECK(math::mod(-7., 3.) == Approx(2.).margin(testutil::MARGIN_FINE));

	CHECK(math::mod(1.5, 2.) == Approx(1.5).margin(testutil::MARGIN_FINE));
	CHECK(math::mod(2.5, 2.) == Approx(0.5).margin(testutil::MARGIN_FINE));
	CHECK(math::mod(-9.1, 8.) == Approx(6.9).margin(testutil::MARGIN_FINE));
}

TEST_CASE("Math - ReverseBitsSimpple")
{
	CHECK(math::reverse_bits(0, 10) == 0);
	CHECK(math::reverse_bits(512, 10) == 1);
	CHECK(math::reverse_bits(256, 10) == 2);
	CHECK(math::reverse_bits(128, 10) == 4);
	CHECK(math::reverse_bits(64, 10) == 8);
	CHECK(math::reverse_bits(32, 10) == 16);
	CHECK(math::reverse_bits(16, 10) == 32);
	CHECK(math::reverse_bits(8, 10) == 64);
	CHECK(math::reverse_bits(4, 10) == 128);
	CHECK(math::reverse_bits(2, 10) == 256);
	CHECK(math::reverse_bits(1, 10) == 512);
}

TEST_CASE("Math - ReverseBitsRandom")
{
	CHECK(math::reverse_bits(0b10011101, 8) == 0b10111001);
	CHECK(math::reverse_bits(0b00101010, 8) == 0b01010100);
	CHECK(math::reverse_bits(0b10101010, 8) == 0b01010101);
	CHECK(math::reverse_bits(0b00011100, 8) == 0b00111000);
	CHECK(math::reverse_bits(0b00110011, 8) == 0b11001100);
	CHECK(math::reverse_bits(0b11001100, 8) == 0b00110011);
	CHECK(math::reverse_bits(0b01010110, 8) == 0b01101010);
	CHECK(math::reverse_bits(0b11101100, 8) == 0b00110111);
}

#endif