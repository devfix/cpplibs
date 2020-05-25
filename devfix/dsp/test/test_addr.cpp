//
// Created by core on 5/25/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include "../addr.h"

using namespace devfix::dsp;

TEST(DSP, AddrSimpple)
{
	ASSERT_EQ((bit_reversed<1024, 0>::value), 0);
	ASSERT_EQ((bit_reversed<1024, 512>::value), 1);
	ASSERT_EQ((bit_reversed<1024, 256>::value), 2);
	ASSERT_EQ((bit_reversed<1024, 128>::value), 4);
	ASSERT_EQ((bit_reversed<1024, 64>::value), 8);
	ASSERT_EQ((bit_reversed<1024, 32>::value), 16);
	ASSERT_EQ((bit_reversed<1024, 16>::value), 32);
	ASSERT_EQ((bit_reversed<1024, 8>::value), 64);
	ASSERT_EQ((bit_reversed<1024, 4>::value), 128);
	ASSERT_EQ((bit_reversed<1024, 2>::value), 256);
	ASSERT_EQ((bit_reversed<1024, 1>::value), 512);
}

TEST(DSP, AddrRandom)
{
	ASSERT_EQ((bit_reversed<256, 0b10011101>::value), 0b10111001);
	ASSERT_EQ((bit_reversed<256, 0b00101010>::value), 0b01010100);
	ASSERT_EQ((bit_reversed<256, 0b10101010>::value), 0b01010101);
	ASSERT_EQ((bit_reversed<256, 0b00011100>::value), 0b00111000);
	ASSERT_EQ((bit_reversed<256, 0b00110011>::value), 0b11001100);
	ASSERT_EQ((bit_reversed<256, 0b11001100>::value), 0b00110011);
	ASSERT_EQ((bit_reversed<256, 0b01010110>::value), 0b01101010);
	ASSERT_EQ((bit_reversed<256, 0b11101100>::value), 0b00110111);
}

#endif