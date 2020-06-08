//
// Created by core on 6/8/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include <cmath>
#include "../interpolation.h"

using namespace devfix::base;

constexpr double ABS_ERROR = 1e-3;

TEST(Interpolation, Eval)
{
	{
		std::vector<double> coeffs = { 11, 7, -5, -4, 2 };
		// all x values are zero -> calculates result like standard horner-scheme for polynomials
		std::vector<std::pair<double, double >> points(coeffs.size());
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, 0), coeffs[0]);
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, 1), 11);
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, 2), 5);
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, 3), 41);
	}

	{
		std::vector<double> coeffs = { 3 };
		// all x values are zero -> calculates result like standard horner-scheme for polynomials
		std::vector<std::pair<double, double >> points(coeffs.size());
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, -2), 3);
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, -1), 3);
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, 0), 3);
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, 1), 3);
		ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, 2), 3);
	}

	ASSERT_ANY_THROW(interpolation<double>::eval({}, {}, 0));  // throws empty coefficients exception

	ASSERT_ANY_THROW(interpolation<double>::eval({}, { 5 }, 0));  // throws length mismatch exception
}

TEST(Interpolation, Coeffs)
{
	{
		std::vector<std::pair<double, double >> points = {
			{ -9, 3 }, { -5, 8 }, { 0, 0 }, { 1, 1 }, { 2, 0 }
		};
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points)
		{
			ASSERT_NEAR(interpolation<double>::eval(points, coeffs, x), y, ABS_ERROR);
		}
	}
	{
		std::vector<std::pair<double, double >> points(64);
		for (int i = 0; i < points.size(); i++)
		{
			double x = 2. * M_PI * (i - int(points.size() / 2)) / points.size();
			points[i] = { x, std::sin(x) };
		}
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points)
		{
			ASSERT_NEAR(interpolation<double>::eval(points, coeffs, x), y, ABS_ERROR);
		}
	}
}

#endif
