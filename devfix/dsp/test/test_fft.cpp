//
// Created by core on 6/1/20.
//


#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include <functional>
#include "../fft.h"

using namespace devfix::dsp;

constexpr double ABS_DOUBLE_ERROR = 1e-12;
constexpr std::size_t LEN = 1024;

template<int digits, typename T>
constexpr T round(T val) { return std::round(val * std::pow(10, digits)) * std::pow(10, -digits); }

TEST(FFT, MultipleFreqs)
{
	std::vector<std::complex<double>> vec(LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = (sin(2 * M_PI * double(i) / double(vec.size()) * 4.)
			+ 0.5 * sin(2 * M_PI * double(i) / double(vec.size()) * 2.)
			+ 0.25 * sin(2 * M_PI * double(i) / double(vec.size()) * 1.)
			+ 4);
	}

	fft::transform_inplace(vec);
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::transform(mag.begin(), mag.end(), mag.begin(), round<3, double>);

	// symmetric test for fft result
	for (std::size_t i = 1; i < LEN / 2; i++) { ASSERT_EQ(mag[i], mag[LEN - i]); }

	// test expected amplitudes
	ASSERT_EQ(mag[0], LEN * 4);
	ASSERT_EQ(mag[1], LEN / 8);
	ASSERT_EQ(mag[2], LEN / 4);
	ASSERT_EQ(mag[3], 0);
	ASSERT_EQ(mag[4], LEN / 2);
	for (std::size_t i = 5; i < LEN / 2; i++) { ASSERT_EQ(mag[i], 0); }
}

TEST(FFT, ApplyWindowPointer)
{
	std::vector<std::complex<double>> vec = { 8, 7, 6, 5, 4, 3, 2, 1 };
	fft::apply_window<double, window::linear, 8>(vec.data());
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::vector<double> expected = {
		0, 2, 3.428571428571428, 4.285714285714286, 4.571428571428571, 4.285714285714286, 3.428571428571428, 2
	};
	ASSERT_EQ(mag.size(), expected.size());
	for (std::size_t i = 0; i < mag.size(); i++) { ASSERT_NEAR(mag[i], expected[i], ABS_DOUBLE_ERROR); }
}

TEST(FFT, ApplyWindowVector)
{
	std::vector<std::complex<double>> vec = { 8, 7, 6, 5, 4, 3, 2, 1 };
	fft::apply_window<double, window::linear, 8>(vec);
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::vector<double> expected = {
		0, 2, 3.428571428571428, 4.285714285714286, 4.571428571428571, 4.285714285714286, 3.428571428571428, 2
	};
	ASSERT_EQ(mag.size(), expected.size());
	for (std::size_t i = 0; i < mag.size(); i++) { ASSERT_NEAR(mag[i], expected[i], ABS_DOUBLE_ERROR); }
}

TEST(FFT, ApplyWindowArray)
{
	std::array<std::complex<double>, 8> vec = { 8, 7, 6, 5, 4, 3, 2, 1 };
	fft::apply_window<double, window::linear, vec.size()>(vec);
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::vector<double> expected = {
		0, 2, 3.428571428571428, 4.285714285714286, 4.571428571428571, 4.285714285714286, 3.428571428571428, 2
	};
	ASSERT_EQ(mag.size(), expected.size());
	for (std::size_t i = 0; i < mag.size(); i++) { ASSERT_NEAR(mag[i], expected[i], ABS_DOUBLE_ERROR); }
}

#endif