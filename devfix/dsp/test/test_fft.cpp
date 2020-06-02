//
// Created by core on 6/1/20.
//


#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include <functional>
#include "../fft.h"

using namespace devfix::dsp;

constexpr std::size_t LEN = 1024;

template<int digits, typename T>
constexpr T round(T val) { return std::round(val * std::pow(10, digits)) * std::pow(10, -digits); }

TEST(FFT, Simple)
{
	std::vector<std::complex<double>> vec(LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = (sin(2 * M_PI * double(i) / double(vec.size()) * 4.)
			+ 0.5 * sin(2 * M_PI * double(i) / double(vec.size()) * 2.)
			+ 0.25 * sin(2 * M_PI * double(i) / double(vec.size()) * 1.)
			+ 4);
	}

	fft::transform_inplace<LEN>(vec.data());
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::transform(mag.begin(), mag.end(), mag.begin(), round<3, double>);

	// symmetric test for fft result
	for (std::size_t i = 1; i < LEN / 2; i++) { ASSERT_EQ(mag[i], mag[LEN-i]); }

	// test expected amplitudes
	ASSERT_EQ(mag[0], LEN * 4);
	ASSERT_EQ(mag[1], LEN / 8);
	ASSERT_EQ(mag[2], LEN / 4);
	ASSERT_EQ(mag[3], 0);
	ASSERT_EQ(mag[4], LEN / 2);
	for (std::size_t i = 5; i < LEN / 2; i++) { ASSERT_EQ(mag[i], 0); }
}

#endif