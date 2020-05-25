//
// Created by core on 5/25/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include <cstring>
#include "../window.h"

using namespace devfix::dsp;

TEST(Window, HanningSmall)
{
	std::vector<double> window = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	std::transform(window.begin(), window.end(), window.begin(), [&window](double v)
	{ return std::round(window::hanning(window.size(), (int)v) * 1e6); });

	std::vector<double>
		expected = { 0, 0.079373233584409, 0.292292493499057, 0.571157419136643, 0.827430366972642, 0.979746486807249,
					 0.979746486807249, 0.827430366972642, 0.571157419136643, 0.292292493499057, 0.079373233584409, 0 };
	std::transform(expected.begin(), expected.end(), expected.begin(), [&expected](double v)
	{ return std::round(v * 1e6); });

	ASSERT_EQ(window.size(), expected.size());
	EXPECT_EQ(window, expected);
}

TEST(Window, HanningLarge)
{
	constexpr std::size_t WINDOW_LENGTH = 8192;
	std::vector<double> window = { 0, 128, 512, 1024, 4096, 8191 };
	std::transform(window.begin(), window.end(), window.begin(), [&window](double v)
	{ return std::round(window::hanning(WINDOW_LENGTH, (int)v) * 1e6); });

	std::vector<double>
		expected = { 0, 0.002408224101074, 0.038069407723687, 0.146480511677018, 0.999999963223881, 0 };
	std::transform(expected.begin(), expected.end(), expected.begin(), [&expected](double v)
	{ return std::round(v * 1e6); });

	ASSERT_EQ(window.size(), expected.size());
	EXPECT_EQ(window, expected);
}

#endif