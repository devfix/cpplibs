//
// Created by core on 6/1/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch2/catch.hpp>
#include <functional>
#include <fstream>
#include "../fft.h"
#include "../window.h"
#include "../winfun.h"
#include "../dsp.h"
#include "../../../testutil.h"

using namespace devfix::base;
using namespace devfix::dsp;

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

TEST_CASE("devfix/dsp/fft/coherent")
{
	std::vector<std::complex<double>> vec(1024);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = 4
			+ 0.25 * std::cos(2 * numbers::pi * double(i) / double(vec.size()) * 1. + 0.1)
			+ 0.50 * std::cos(2 * numbers::pi * double(i) / double(vec.size()) * 2. + 0.2)
			+ 1.00 * std::cos(2 * numbers::pi * double(i) / double(vec.size()) * 4. + 0.3);
	}

	fft::transform_inplace(vec);
	vec.resize(vec.size() / 2);
	fft::normalize_inplace(vec, vec.size() * 2);
	const auto mag = fft::get_magnitude(vec);
	const auto ph = fft::get_phase(vec);

	// test magnitudes
	CHECK(mag[0] == Approx(4).margin(testutil::MARGIN_FINE));
	CHECK(mag[1] == Approx(0.25).margin(testutil::MARGIN_FINE));
	CHECK(mag[2] == Approx(0.5).margin(testutil::MARGIN_FINE));
	CHECK(mag[3] == Approx(0).margin(testutil::MARGIN_FINE));
	CHECK(mag[4] == Approx(1.).margin(testutil::MARGIN_FINE));
	for (std::size_t i = 5; i < mag.size(); i++) { CHECK(mag[i] == Approx(0).margin(testutil::MARGIN_FINE)); }

	// test phases
	CHECK(ph[0] == Approx(0).margin(testutil::MARGIN_FINE));
	CHECK(ph[1] == Approx(0.1).margin(testutil::MARGIN_FINE));
	CHECK(ph[2] == Approx(0.2).margin(testutil::MARGIN_FINE));
	CHECK(ph[4] == Approx(0.3).margin(testutil::MARGIN_FINE));
}

TEST_CASE("devfix/dsp/fft/large_len")
{
	constexpr std::size_t N = 1u << 16u;
	constexpr double fs = 44100;
	std::vector<std::complex<double>> vec(N);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		const double t = double(i) / fs;
		vec[i] = 1.2
			+ 1.0 * std::cos(2 * numbers::pi * 220 * t + 0.1)
			+ 0.8 * std::cos(2 * numbers::pi * 440 * t + 0.2)
			+ 0.6 * std::cos(2 * numbers::pi * 880 * t + 0.3);
	}

	window win(winfun::flattop_hft248d<double>, vec.size(), true);
	win.apply(vec);
	fft::transform_inplace(vec);
	vec.resize(N / 2);
	fft::normalize_inplace(vec, N);

	const auto bin220 = calcfreqbin(fs, N, 220.);
	const auto idx220 = calcfreqidx(fs, N, 220.);
	const auto bin440 = calcfreqbin(fs, N, 440.);
	const auto idx440 = calcfreqidx(fs, N, 440.);
	const auto bin880 = calcfreqbin(fs, N, 880.);
	const auto idx880 = calcfreqidx(fs, N, 880.);

	// test magnitudes
	const auto mag = fft::get_magnitude(vec);
	CHECK(mag[0] == Approx(1.2).margin(testutil::MARGIN_COARSE));
	CHECK(mag[idx220] == Approx(1.0).margin(testutil::MARGIN_COARSE));
	CHECK(mag[idx440] == Approx(0.8).margin(testutil::MARGIN_COARSE));
	CHECK(mag[idx880] == Approx(0.6).margin(testutil::MARGIN_COARSE));

	// correct phases inplace
	vec[idx220] *= calcphasecorrector(fs, N, 220.);
	vec[idx440] *= calcphasecorrector(fs, N, 440.);
	vec[idx880] *= calcphasecorrector(fs, N, 880.);

	// test phases
	const auto ph = fft::get_phase(vec);
	CHECK(ph[idx220] == Approx(0.1).margin(testutil::MARGIN_COARSE));
	CHECK(ph[idx440] == Approx(0.2).margin(testutil::MARGIN_COARSE));
	CHECK(ph[idx880] == Approx(0.3).margin(testutil::MARGIN_COARSE));
}

#endif