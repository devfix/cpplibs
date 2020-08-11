//
// Created by core on 7/9/20.
//


#if CPPLIBS_ENABLE_TESTS == 1

#include <catch2/catch.hpp>
#include <iostream>
#include <array>
#include "../type.h"

using namespace devfix::base;

enum class Enum {};  // dummy enum
struct Struct {};  // dummy struct

TEST_CASE("type - demangle")
{
	CHECK(type::demangle(typeid(std::invalid_argument("")).name()) == "std::invalid_argument");
	CHECK(type::demangle(typeid(std::runtime_error("")).name()) == "std::runtime_error");
	CHECK(type::demangle(typeid(8).name()) == "int");
	CHECK(type::demangle(typeid(1.1).name()) == "double");
	CHECK(type::demangle(typeid(1.f).name()) == "float");
	CHECK(type::demangle(typeid(true).name()) == "bool");
	CHECK(type::demangle(typeid(Enum).name()) == "Enum");
	CHECK(type::demangle(typeid(Enum*).name()) == "Enum*");
	CHECK(type::demangle(typeid(Struct).name()) == "Struct");
	CHECK(type::demangle(typeid(Struct*).name()) == "Struct*");
	CHECK(type::demangle(typeid(std::array<Struct, 8>).name()) == "std::array<Struct,8>");
	CHECK(type::demangle(typeid(char*).name()) == "char*");
	CHECK(type::demangle(typeid(char**).name()) == "char**");
	CHECK(type::demangle(typeid(std::array<char*,4>).name()) == "std::array<char*,4>");
	CHECK(type::demangle(typeid(std::vector<int>).name()) == "std::vector<int,std::allocator<int>>");
	CHECK(type::demangle(typeid(std::vector<std::vector<Enum*>*>*).name())
			  == "std::vector<std::vector<Enum*,std::allocator<Enum*>>*,std::allocator<std::vector<Enum*,std::allocator<Enum*>>*>>*");
	CHECK(type::demangle(typeid(std::unique_ptr<int>).name()) == "std::unique_ptr<int,std::default_delete<int>>");
	CHECK(type::demangle(typeid(std::unique_ptr<std::shared_ptr<float>>).name())
			  == "std::unique_ptr<std::shared_ptr<float>,std::default_delete<std::shared_ptr<float>>>");
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
