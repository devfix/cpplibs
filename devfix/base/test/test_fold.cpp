//
// Created by core on 12/19/19.
//

#include <gtest/gtest.h>
#include <iostream>
#include "../foldt.h"

template<typename T>
constexpr T add(T a, T b)
{
	return a + b;
}

template<typename T, T d>
constexpr T incIfDiv(T a, T b)
{
	return a + !(b % d);
}

TEST(Fold, Template)
{
	ASSERT_EQ(foldt(0, add<int>, 1, 2, 3, 4), 10);
	ASSERT_EQ(foldt(0, add<int>), 0);
	ASSERT_EQ(foldt(5, add<int>), 5);
	ASSERT_EQ(foldt(0, incIfDiv<int, 3>, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9), 4);
}