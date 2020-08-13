//
// Created by core on 6/1/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch2/catch.hpp>
#include <functional>
#include <fstream>
#include "../fft.h"
#include "../window.h"
#include "../../../testutil.h"
#include "../winfun.h"

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
			+ 0.25 * std::cos(2 * math::pi * double(i) / double(vec.size()) * 1. + 0.1)
			+ 0.50 * std::cos(2 * math::pi * double(i) / double(vec.size()) * 2. + 0.2)
			+ 1.00 * std::cos(2 * math::pi * double(i) / double(vec.size()) * 4. + 0.3);
	}

	fft::transform_inplace(vec);
	const auto magphase = fft::get_mag_and_phase(vec);

	// test magnitudes
	CHECK(magphase[0].first == Approx(4).margin(testutil::MARGIN_FINE));
	CHECK(magphase[1].first == Approx(0.25).margin(testutil::MARGIN_FINE));
	CHECK(magphase[2].first == Approx(0.5).margin(testutil::MARGIN_FINE));
	CHECK(magphase[3].first == Approx(0).margin(testutil::MARGIN_FINE));
	CHECK(magphase[4].first == Approx(1.).margin(testutil::MARGIN_FINE));
	for (std::size_t i = 5; i < magphase.size(); i++) { CHECK(magphase[i].first == Approx(0).margin(testutil::MARGIN_FINE)); }

	// test phases
	CHECK(magphase[0].second == Approx(0).margin(testutil::MARGIN_FINE));
	CHECK(magphase[1].second == Approx(0.1).margin(testutil::MARGIN_FINE));
	CHECK(magphase[2].second == Approx(0.2).margin(testutil::MARGIN_FINE));
	CHECK(magphase[4].second == Approx(0.3).margin(testutil::MARGIN_FINE));
}

TEST_CASE("devfix/dsp/fft/large_len")
{
	constexpr double fs = 44100;
	std::vector<std::complex<double>> vec(1u << 16u);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		const double t = double(i) / fs;
		vec[i] = (std::cos(2 * math::pi * 440 * t + 0.3 * math::pi)
			+ 0.8 * std::cos(2 * math::pi * 220 * t + 0.2 * math::pi)
			+ 0.6 * std::cos(2 * math::pi * 880 * t + 0.3 * math::pi)
			+ 0.4);
	}

	window win(winfun<double>::flattop_hft248d, vec.size(), true);
	win.apply(vec);
	fft::transform_inplace(vec);
	const auto magphase = fft::get_mag_and_phase(vec);

	const std::size_t line_440 = std::round(440. * vec.size() / fs);
	const std::size_t line_220 = std::round(220. * vec.size() / fs);
	const std::size_t line_880 = std::round(880. * vec.size() / fs);

	// test magnitudes
	CHECK(magphase[line_440].first == Approx(1).margin(testutil::MARGIN_COARSE));
	CHECK(magphase[line_220].first == Approx(0.8).margin(testutil::MARGIN_COARSE));
	CHECK(magphase[line_880].first == Approx(0.6).margin(testutil::MARGIN_COARSE));
	CHECK(magphase[0].first == Approx(0.4).margin(testutil::MARGIN_COARSE));

	// phases cannot be testedco with this window
}

#endif