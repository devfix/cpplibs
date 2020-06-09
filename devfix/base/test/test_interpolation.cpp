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
			{ -2, 4, }, { -1, 1 }, { 0, 0 }, { 1, 1 }, { 2, 4 }
		};
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points)
		{
			ASSERT_DOUBLE_EQ(interpolation<double>::eval(points, coeffs, x), y);
		}
	}

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
			double x = M_PI * (i - int(points.size() / 2)) / points.size();
			points[i] = { x, std::sin(x) };
		}
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points)
		{
			ASSERT_NEAR(interpolation<double>::eval(points, coeffs, x), y, ABS_ERROR);
		}
	}

	{
		std::vector<std::pair<double, double >> points(16);
		for (int i = 0; i < points.size(); i++)
		{
			points[i] = { i, std::sin(i) };
		}
		auto coeffs = interpolation<double>::calc_coeffs(points);
		for (auto&[x, y] : points)
		{
			ASSERT_NEAR(interpolation<double>::eval(points, coeffs, x), y, ABS_ERROR);
		}
	}
}

TEST(Interpolation, BiSecReg)
{
	constexpr double abs_error = 1e-10;
	{
		std::vector<std::pair<double, double >> points = {
			{ -2, 4, }, { -1, 1 }, { 0, 0 }, { 1, 1 }, { 2, 4 }
		};
		auto coeffs = interpolation<double>::calc_coeffs(points);

		double x1 = interpolation<double>::bisecreg(points, coeffs, points.begin()->first, 0, 0.25, abs_error);
		ASSERT_NEAR(x1, -0.5, abs_error);
		double x2 = interpolation<double>::bisecreg(points, coeffs, 0, (--points.end())->first, 0.25, abs_error);
		ASSERT_NEAR(x2, 0.5, abs_error);

		double x3 = interpolation<double>::bisecreg(points, coeffs, points.begin()->first, 0, 2.25, abs_error);
		ASSERT_NEAR(x3, -1.5, abs_error);
		double x4 = interpolation<double>::bisecreg(points, coeffs, 0, (--points.end())->first, 2.25, abs_error);
		ASSERT_NEAR(x4, 1.5, abs_error);

		double x5 = interpolation<double>::bisecreg(points, coeffs, 0, 1, 1e-8, abs_error);
		ASSERT_NEAR(x5, 0.00010013580322265625, abs_error);
	}
}


#endif
