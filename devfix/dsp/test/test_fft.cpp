//
// Created by core on 6/1/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch2/catch.hpp>
#include <functional>
#include "../fft.h"
#include "../window.h"

using namespace devfix::base;
using namespace devfix::dsp;

static constexpr std::size_t LEN = 1024;

template<int digits, typename T>
constexpr T round(T val) { return std::round(val * std::pow(10, digits)) * std::pow(10, -digits); }

TEST_CASE("devfix/dsp/fft/exception")
{
	{
		std::vector<std::complex<double>> vec(9);
		CHECK_THROWS(fft::transform_inplace(vec));
	}
	{
		std::vector<std::complex<double>> vec(7);
		CHECK_THROWS(fft::transform_inplace(vec));
	}
	{
		std::vector<std::complex<double>> vec(99);
		CHECK_THROWS(fft::transform_inplace(vec));
	}
}

TEST_CASE("devfix/dsp/fft/multiple_freqs")
{
	std::vector<std::complex<double>> vec(LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = (sin(2 * math::pi * double(i) / double(vec.size()) * 4.)
			+ 0.5 * std::sin(2 * math::pi * double(i) / double(vec.size()) * 2.)
			+ 0.25 * std::sin(2 * math::pi * double(i) / double(vec.size()) * 1.)
			+ 4);
	}

	fft::transform_inplace(vec);
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::transform(mag.begin(), mag.end(), mag.begin(), round<3, double>);

	// symmetric test for fft result
	for (std::size_t i = 1; i < LEN / 2; i++) { CHECK(mag[i] == mag[LEN - i]); }

	// test expected amplitudes
	CHECK(mag[0] == Approx(4).margin(PRECISION_FINE));
	CHECK(mag[1] * 2 == Approx(0.25).margin(PRECISION_FINE));
	CHECK(mag[2] * 2 == Approx(0.5).margin(PRECISION_FINE));
	CHECK(mag[3] * 2 == Approx(0).margin(PRECISION_FINE));
	CHECK(mag[4] * 2 == Approx(1.).margin(PRECISION_FINE));
	for (std::size_t i = 5; i < LEN / 2; i++) { CHECK(mag[i] * 2 == Approx(0).margin(PRECISION_FINE)); }
}

TEST_CASE("FFT - ApplyWindowPointer")
{
	std::vector<std::complex<double>> vec = { 8, 7, 6, 5, 4, 3, 2, 1 };
	apply_window<double, window::linear>(vec.data(), vec.size());
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::vector<double> expected = {
		0, 2, 3.428571428571428, 4.285714285714286, 4.571428571428571, 4.285714285714286, 3.428571428571428, 2
	};
	REQUIRE(mag.size() == expected.size());
	for (std::size_t i = 0; i < mag.size(); i++) { CHECK(mag[i] == Approx(expected[i]).margin(PRECISION_FINE)); }
}

TEST_CASE("FFT - ApplyWindowVector")
{
	std::vector<std::complex<double>> vec = { 8, 7, 6, 5, 4, 3, 2, 1 };
	apply_window<double, window::linear>(vec);
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::vector<double> expected = {
		0, 2, 3.428571428571428, 4.285714285714286, 4.571428571428571, 4.285714285714286, 3.428571428571428, 2
	};
	REQUIRE(mag.size() == expected.size());
	for (std::size_t i = 0; i < mag.size(); i++) { CHECK(mag[i] == Approx(expected[i]).margin(PRECISION_FINE)); }
}

TEST_CASE("FFT - ApplyWindowArray")
{
	std::array<std::complex<double>, 8> vec = { 8, 7, 6, 5, 4, 3, 2, 1 };
	apply_window<double, window::linear>(vec);
	std::vector<double> mag(vec.size());
	std::transform(vec.begin(), vec.end(), mag.begin(), std::abs<double>);
	std::vector<double> expected = {
		0, 2, 3.428571428571428, 4.285714285714286, 4.571428571428571, 4.285714285714286, 3.428571428571428, 2
	};
	REQUIRE(mag.size() == expected.size());
	for (std::size_t i = 0; i < mag.size(); i++) { CHECK(mag[i] == Approx(expected[i]).margin(PRECISION_FINE)); }
}

TEST_CASE("FFT - AmplitudeNormalization")
{
	std::vector<std::complex<double>> vec(128);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = (sin(2 * math::pi * double(i) / double(vec.size()) * 4.)
			+ 0.5 * std::sin(2 * math::pi * double(i) / double(vec.size()) * 2.)
			+ 0.25 * std::sin(2 * math::pi * double(i) / double(vec.size()) * 1.)
			+ 4);
	}

	fft::transform_inplace(vec);
	auto positive = fft::convert_to_onesided(vec);
	std::vector<double> mag(positive.size());
	std::transform(positive.begin(), positive.end(), mag.begin(), std::abs<double>);

	std::vector<double> expected =
		{ 4, 0.25, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
	REQUIRE(positive.size() == expected.size());
	for (std::size_t i = 0; i < positive.size(); i++) { CHECK(mag[i] == Approx(expected[i]).margin(PRECISION_FINE)); }
}

TEST_CASE("FFT - AmplitudeNormalizationWithFlatTop")
{
	constexpr std::size_t FFT_LEN = 128;
	std::vector<std::complex<double>> vec(FFT_LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = std::sin(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 32)
			+ 0.5 * std::sin(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 8)
			+ 0.25 * std::sin(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 4);
	}

	apply_window<double, window::flattop>(vec);
	fft::transform_inplace(vec);
	auto positive = fft::convert_to_onesided(vec);
	std::vector<double> mag(positive.size());
	std::transform(positive.begin(), positive.end(), mag.begin(), std::abs<double>);

	std::vector<double> expected =
		{ 0.00175583, 0.201761, 0.650524, 0.967362, 1.00001, 0.967367, 0.650518, 0.201786, 0.0179323, 3.18503e-05, 1.60198e-05, 2.79171e-05,
		  0.0089526, 0.100907, 0.325245, 0.483698, 0.499992, 0.483696, 0.325248, 0.100902, 0.00895885, 2.19893e-05, 1.4057e-05, 1.57144e-05,
		  1.56228e-05, 1.46173e-05, 1.26949e-05, 1.55266e-05, 0.00447605, 0.0504534, 0.162622, 0.241849, 0.249996, 0.241848, 0.162624,
		  0.0504507, 0.00447977, 1.04789e-05, 6.275e-06, 6.82469e-06, 6.52186e-06, 5.91506e-06, 5.27451e-06, 4.68813e-06, 4.17657e-06,
		  3.73776e-06, 3.36303e-06, 3.04274e-06, 2.76813e-06, 2.53178e-06, 2.32763e-06, 2.15073e-06, 1.99712e-06, 1.86362e-06, 1.74766e-06,
		  1.64719e-06, 1.56058e-06, 1.48654e-06, 1.42403e-06, 1.37227e-06, 1.33064e-06, 1.29868e-06, 1.27607e-06, 1.26258e-06,
		};
	REQUIRE(mag.size() == expected.size());
	for (std::size_t i = 0; i < mag.size(); i++) { CHECK(mag[i] == Approx(expected[i]).margin(PRECISION_COARSE)); }
}

TEST_CASE("FFT - PhaseExtraction")
{
	constexpr std::size_t FFT_LEN = 128;
	std::vector<std::complex<double>> vec(FFT_LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = std::cos(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 32 + math::pi)
			+ 0.5 * std::cos(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 8 + math::pi / 2)
			+ 0.25 * std::cos(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 4 + math::pi / 4);
	}

	apply_window<double, window::rectangle>(vec);
	fft::transform_inplace(vec);
	auto positive = fft::convert_to_onesided(vec);
	auto angles = fft::extract_angles(positive, 0.1);

	constexpr std::size_t line_a = FFT_LEN / 32, line_b = FFT_LEN / 8, line_c = FFT_LEN / 4;
	CHECK(angles[line_a] == Approx(math::pi));
	CHECK(angles[line_b] == Approx(math::pi / 2));
	CHECK(angles[line_c] == Approx(math::pi / 4));
	for (std::size_t i = 0; i < angles.size(); i++)
	{
		if (i != line_a && i != line_b && i != line_c) { CHECK(angles[i] == Approx(0).margin(PRECISION_FINE)); }
	}
}

#endif