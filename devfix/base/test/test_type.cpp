//
// Created by core on 7/9/20.
//


#if CPPLIBS_ENABLE_TESTS == 1

#include <catch.hpp>
#include <iostream>
#include "../type.h"

using namespace devfix::base;

TEST_CASE("type - demangle")
{
	CHECK(type::demangle(typeid(std::invalid_argument("")).name()) == "std::invalid_argument");
	CHECK(type::demangle(typeid(std::runtime_error("")).name()) == "std::runtime_error");
	CHECK(type::demangle(typeid(8).name()) == "int");
	CHECK(type::demangle(typeid(1.1).name()) == "double");
	CHECK(type::demangle(typeid(1.f).name()) == "float");
	CHECK(type::demangle(typeid(true).name()) == "bool");
	CHECK(type::demangle(typeid(char*).name()) == "char*");
	CHECK(type::demangle(typeid(std::vector<int>).name()) == "std::vector<int, std::allocator<int> >");
	CHECK(type::demangle(typeid(std::unique_ptr<int>).name()) == "std::unique_ptr<int, std::default_delete<int> >");
}

TEST_CASE("type - removed nested")
{
	try
	{
		try { throw std::range_error("1"); }
		catch (...) { std::throw_with_nested(std::runtime_error("2")); }
	}
	catch (const std::exception& ex)
	{
		CHECK(type::remove_nested(type::demangle(typeid(ex).name())) == "std::runtime_error");
	}

	try
	{
		try { throw std::range_error("1"); }
		catch (...) { std::throw_with_nested(std::invalid_argument("2")); }
	}
	catch (const std::exception& ex)
	{
		CHECK(type::remove_nested(type::demangle(typeid(ex).name())) == "std::invalid_argument");
	}
}

#endif
