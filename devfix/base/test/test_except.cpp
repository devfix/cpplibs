//
// Created by core on 7/9/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch/catch.hpp>
#include "../except.h"

using namespace devfix::base;

TEST_CASE("except - get_trace")
{
	try
	{
		try
		{
			try { throw std::range_error("range error"); }
			catch (...) { std::throw_with_nested(std::invalid_argument("invalid argument")); }
		}
		catch (...) { std::throw_with_nested(std::runtime_error("runtime error")); }
	}
	catch (...)
	{
		auto trace = except::get_trace();
		REQUIRE(trace.size() == 3);
		REQUIRE(trace.front().has_value());
		CHECK(trace.front().value().first == "std::runtime_error");
		CHECK(trace.front().value().second == "runtime error");
		trace.pop();
		REQUIRE(trace.front().has_value());
		CHECK(trace.front().value().first == "std::invalid_argument");
		CHECK(trace.front().value().second == "invalid argument");
		trace.pop();
		REQUIRE(trace.front().has_value());
		CHECK(trace.front().value().first == "std::range_error");
		CHECK(trace.front().value().second == "range error");
	}

}

#endif