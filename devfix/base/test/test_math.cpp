//
// Created by core on 5/25/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include "../math.h"

using namespace devfix::base;

TEST(Math, Log2)
{
	ASSERT_EQ(math::popcount(1u), 0);
	ASSERT_EQ(math::popcount(2u), 1);
	ASSERT_EQ(math::popcount(4u), 2);
	ASSERT_EQ(math::popcount(8u), 3);
	ASSERT_EQ(math::popcount(16u), 4);
	ASSERT_EQ(math::popcount(32u), 5);
	ASSERT_EQ(math::popcount(64u), 6);
	ASSERT_EQ(math::popcount(128u), 7);
}

TEST(Math, ReverseBitsSimpple)
{
	ASSERT_EQ(math::reverse_bits(0, 10), 0);
	ASSERT_EQ(math::reverse_bits(512, 10), 1);
	ASSERT_EQ(math::reverse_bits(256, 10), 2);
	ASSERT_EQ(math::reverse_bits(128, 10), 4);
	ASSERT_EQ(math::reverse_bits(64, 10), 8);
	ASSERT_EQ(math::reverse_bits(32, 10), 16);
	ASSERT_EQ(math::reverse_bits(16, 10), 32);
	ASSERT_EQ(math::reverse_bits(8, 10), 64);
	ASSERT_EQ(math::reverse_bits(4, 10), 128);
	ASSERT_EQ(math::reverse_bits(2, 10), 256);
	ASSERT_EQ(math::reverse_bits(1, 10), 512);
}

TEST(Math, ReverseBitsRandom)
{
	ASSERT_EQ(math::reverse_bits(0b10011101, 8), 0b10111001);
	ASSERT_EQ(math::reverse_bits(0b00101010, 8), 0b01010100);
	ASSERT_EQ(math::reverse_bits(0b10101010, 8), 0b01010101);
	ASSERT_EQ(math::reverse_bits(0b00011100, 8), 0b00111000);
	ASSERT_EQ(math::reverse_bits(0b00110011, 8), 0b11001100);
	ASSERT_EQ(math::reverse_bits(0b11001100, 8), 0b00110011);
	ASSERT_EQ(math::reverse_bits(0b01010110, 8), 0b01101010);
	ASSERT_EQ(math::reverse_bits(0b11101100, 8), 0b00110111);
}

#endif