//
// Created by core on 8/12/20.
//

#pragma once

#include <catch2/catch.hpp>

struct testutil
{
	static constexpr double MARGIN_FINE = 1e-6;
	static constexpr double MARGIN_COARSE = 1e-3;

	template<typename T>
	static void check_equals(const T* a, const T* b, double margin, std::size_t len)
	{
		REQUIRE(len > 0);
		for (std::size_t i = 0; i < len; i++) { CHECK(a[i] == Approx(b[i]).margin(margin)); }
	}

	template<typename T>
	static void check_equals(const std::vector<T>& a, const T* b, double margin)
	{
		check_equals<T>(a.data(), b, a.size(), margin);
	}

	template<typename T>
	static void check_equals(const T* a, const std::vector<T>& b, double margin)
	{
		check_equals<T>(a, b.data(), b.size(), margin);
	}

	template<typename T>
	static void check_equals(const std::vector<T>& a, const std::vector<T>& b, double margin)
	{
		REQUIRE(a.size() == b.size());
		check_equals<T>(a.data(), b.data(), margin, a.size());
	}
};
