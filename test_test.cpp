//
// Created by core on 6/28/20.
//

#include <catch/catch.hpp>

TEST_CASE("catch test")
{
	REQUIRE(false);
	SECTION("testus")
	{
		REQUIRE_FALSE(false);
	}
}
