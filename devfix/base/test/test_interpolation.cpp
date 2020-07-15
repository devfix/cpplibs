//
// Created by core on 6/8/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch/catch.hpp>
#include <cmath>
#include "../interpolation.h"
#include "../math.h"

static constexpr double PRECISION_FINE = 1e-6;
static constexpr double PRECISION_COARSE = 1e-3;

using namespace devfix::base;

TEST_CASE("Interpolation - Eval")
{
	{
		std::vector<double> coeffs = { 11, 7, -5, -4, 2 };
		// all x values are zero -> calculates result like standard horner-scheme for polynomials
		std::vector<std::pair<double, double >> points(coeffs.size());
		REQUIRE(interpolation<double>::eval(points, coeffs, 0) == Approx(coeffs[0]));
		REQUIRE(interpolation<double>::eval(points, coeffs, 1) == Approx(11));
		REQUIRE(interpolation<double>::eval(points, coeffs, 2) == Approx(5));
		REQUIRE(interpolation<double>::eval(points, coeffs, 3) == Approx(41));
	}

	{
		std::vector<double> coeffs = { 3 };
		// all x values are zero -> calculates result like standard horner-scheme for polynomials
		std::vector<std::pair<double, double >> points(coeffs.size());
		REQUIRE(interpolation<double>::eval(points, coeffs, -2) == Approx(3));
		REQUIRE(interpolation<double>::eval(points, coeffs, -1) == Approx(3));
		REQUIRE(interpolation<double>::eval(points, coeffs, 0) == Approx(3));
		REQUIRE(interpolation<double>::eval(points, coeffs, 1) == Approx(3));
		REQUIRE(interpolation<double>::eval(points, coeffs, 2) == Approx(3));
	}

	REQUIRE_THROWS(interpolation<double>::eval(std::vector<std::pair<double, double >>{}, {}, 0));  // throws empty coefficients exception

	REQUIRE_THROWS(interpolation<double>::eval(std::vector<std::pair<double, double >>{}, { 5 }, 0));  // throws length mismatch exception
}

TEST_CASE("Interpolation - Coeffs")
{
	{
		std::vector<std::pair<double, double >> points = {
			{ -2, 4, }, { -1, 1 }, { 0, 0 }, { 1, 1 }, { 2, 4 }
		};
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points) { REQUIRE(interpolation<double>::eval(points, coeffs, x) == Approx(y).epsilon(PRECISION_FINE)); }
	}

	{
		std::vector<std::pair<double, double >> points = {
			{ -9, 3 }, { -5, 8 }, { 0, 0 }, { 1, 1 }, { 2, 0 }
		};
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points) { REQUIRE(interpolation<double>::eval(points, coeffs, x) == Approx(y).margin(PRECISION_COARSE)); }
	}

	{
		std::vector<std::pair<double, double >> points(64);
		for (int i = 0; i < points.size(); i++)
		{
			double x = math::pi * (i - int(points.size() / 2)) / points.size();
			points[i] = { x, std::sin(x) };
		}
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points) { REQUIRE(interpolation<double>::eval(points, coeffs, x) == Approx(y).epsilon(PRECISION_COARSE)); }
	}

	{
		std::vector<std::pair<double, double >> points(16);
		for (int i = 0; i < points.size(); i++)
		{
			points[i] = { i, std::sin(i) };
		}
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points) { REQUIRE(interpolation<double>::eval(points, coeffs, x) == Approx(y).epsilon(PRECISION_COARSE)); }
	}
}

TEST_CASE("Interpolation - BiSecReg")
{
	constexpr double abs_error = 1e-10;
	{
		std::vector<std::pair<double, double >> points = {
			{ -2, 4, }, { -1, 1 }, { 0, 0 }, { 1, 1 }, { 2, 4 }
		};
		auto coeffs = interpolation<double>::calc_coeffs(points);

		double x1 = interpolation<double>::bisec(points, coeffs, points.begin()->first, 0, 0.25, abs_error);
		REQUIRE(x1 == Approx(-0.5).epsilon(PRECISION_FINE));
		double x2 = interpolation<double>::bisec(points, coeffs, 0, (--points.end())->first, 0.25, abs_error);
		REQUIRE(x2 == Approx(0.5).epsilon(PRECISION_FINE));

		double x3 = interpolation<double>::bisec(points, coeffs, points.begin()->first, 0, 2.25, abs_error);
		REQUIRE(x3 == Approx(-1.5).epsilon(PRECISION_FINE));
		double x4 = interpolation<double>::bisec(points, coeffs, 0, (--points.end())->first, 2.25, abs_error);
		REQUIRE(x4 == Approx(1.5).epsilon(PRECISION_FINE));

		double x5 = interpolation<double>::bisec(points, coeffs, 0, 1, 1e-8, abs_error);
		REQUIRE(x5 == Approx(0.00010013580322265625).epsilon(PRECISION_FINE));
	}
}

#endif
