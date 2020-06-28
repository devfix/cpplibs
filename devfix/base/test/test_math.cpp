//
// Created by core on 5/25/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch/catch.hpp>
#include "../math.h"

using namespace devfix::base;

TEST_CASE("Math - Log2")
{
	REQUIRE(math::popcount(1u) == 0);
	REQUIRE(math::popcount(2u) == 1);
	REQUIRE(math::popcount(4u) == 2);
	REQUIRE(math::popcount(8u) == 3);
	REQUIRE(math::popcount(16u) == 4);
	REQUIRE(math::popcount(32u) == 5);
	REQUIRE(math::popcount(64u) == 6);
	REQUIRE(math::popcount(128u) == 7);
}

TEST_CASE("Math - ReverseBitsSimpple")
{
	REQUIRE(math::reverse_bits(0, 10) == 0);
	REQUIRE(math::reverse_bits(512, 10) == 1);
	REQUIRE(math::reverse_bits(256, 10) == 2);
	REQUIRE(math::reverse_bits(128, 10) == 4);
	REQUIRE(math::reverse_bits(64, 10) == 8);
	REQUIRE(math::reverse_bits(32, 10) == 16);
	REQUIRE(math::reverse_bits(16, 10) == 32);
	REQUIRE(math::reverse_bits(8, 10) == 64);
	REQUIRE(math::reverse_bits(4, 10) == 128);
	REQUIRE(math::reverse_bits(2, 10) == 256);
	REQUIRE(math::reverse_bits(1, 10) == 512);
}

TEST_CASE("Math - ReverseBitsRandom")
{
	REQUIRE(math::reverse_bits(0b10011101, 8) == 0b10111001);
	REQUIRE(math::reverse_bits(0b00101010, 8) == 0b01010100);
	REQUIRE(math::reverse_bits(0b10101010, 8) == 0b01010101);
	REQUIRE(math::reverse_bits(0b00011100, 8) == 0b00111000);
	REQUIRE(math::reverse_bits(0b00110011, 8) == 0b11001100);
	REQUIRE(math::reverse_bits(0b11001100, 8) == 0b00110011);
	REQUIRE(math::reverse_bits(0b01010110, 8) == 0b01101010);
	REQUIRE(math::reverse_bits(0b11101100, 8) == 0b00110111);
}

#endif