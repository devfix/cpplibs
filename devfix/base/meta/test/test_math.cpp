//
// Created by core on 5/25/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include "../math.h"

using namespace devfix::base::meta;

TEST(Math, Log2)
{
	ASSERT_EQ(math::log2<1>::value, 0);
	ASSERT_EQ(math::log2<2>::value, 1);
	ASSERT_EQ(math::log2<4>::value, 2);
	ASSERT_EQ(math::log2<8>::value, 3);
	ASSERT_EQ(math::log2<16>::value, 4);
	ASSERT_EQ(math::log2<32>::value, 5);
	ASSERT_EQ(math::log2<64>::value, 6);
	ASSERT_EQ(math::log2<128>::value, 7);
}

#endif