//
// Created by core on 12/19/19.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch2/catch.hpp>
#include "../foldt.h"

template<typename T>
constexpr T add(T a, T b) { return a + b; }

template<typename T, T d>
constexpr T incIfDiv(T a, T b) { return a + !(b % d); }

TEST_CASE("Fold - Template")
{
	REQUIRE(foldt(0, add<int>, 1, 2, 3, 4) == 10);
	REQUIRE(foldt(0, add<int>) == 0);
	REQUIRE(foldt(5, add<int>) == 5);
	REQUIRE(foldt(0, incIfDiv<int, 3>, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 4);
}

#endif
