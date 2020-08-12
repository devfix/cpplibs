//
// Created by core on 6/2/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch2/catch.hpp>
#include "../strutil.h"

using namespace devfix::base;
using namespace std::string_literals;

TEST_CASE("StrUtil - find_regex")
{
	{
		auto res = strutil::find_regex("abc6546def"s, "c[0-9]*d"s);
		REQUIRE(res.has_value());
		CHECK(res->first == 2);
		CHECK(res->second == 6);
	}
	{
		auto res = strutil::find_regex("test<class uff>"s, "((class)|(struct)|(enum)) "s);
		REQUIRE(res.has_value());
		CHECK(res->first == 5);
		CHECK(res->second == 6);
	}
}


#endif