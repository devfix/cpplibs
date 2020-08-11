//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch2/catch.hpp>
#include "../dsp.h"
#include "../../base/math.h"

static constexpr double PRECISION_FINE = 1e-6;

using namespace devfix::base;
using namespace devfix::dsp;

TEST_CASE("DSP - Goertzel Amplitude")
{
	constexpr std::size_t LEN = 1024;
	std::vector<std::complex<double>> vec(LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = (sin(2 * math::pi * double(i) / double(vec.size()) * 4.)
			+ 0.5 * std::sin(2 * math::pi * double(i) / double(vec.size()) * 2.)
			+ 0.25 * std::sin(2 * math::pi * double(i) / double(vec.size()) * 1.)
			+ 4);
	}

	// test expected amplitudes
	CHECK(std::abs(goertzel(vec, 0)) == Approx(4).epsilon(PRECISION_FINE));
	CHECK(std::abs(goertzel(vec, 1)) * 2 == Approx(0.25).epsilon(PRECISION_FINE));
	CHECK(std::abs(goertzel(vec, 2)) * 2 == Approx(0.5).epsilon(PRECISION_FINE));
	CHECK(std::abs(goertzel(vec, 3)) * 2 == Approx(0).margin(PRECISION_FINE));
	CHECK(std::abs(goertzel(vec, 4)) * 2 == Approx(1).epsilon(PRECISION_FINE));
	for (std::size_t i = 5; i < LEN / 2; i++) { CHECK(2 * std::abs(goertzel(vec, i)) == Approx(0).margin(PRECISION_FINE)); }
}

TEST_CASE("DSP - Goertzel Phases")
{
	constexpr std::size_t FFT_LEN = 128;
	std::vector<std::complex<double>> vec(FFT_LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = std::cos(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 32 + math::pi)
			+ 0.5 * std::cos(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 8 + math::pi / 2)
			+ 0.25 * std::cos(2 * math::pi * double(i) / double(vec.size()) * FFT_LEN / 4 + math::pi / 4);
	}

	constexpr std::size_t line_a = FFT_LEN / 32, line_b = FFT_LEN / 8, line_c = FFT_LEN / 4;
	CHECK(std::arg(goertzel(vec, line_a)) == Approx(math::pi));
	CHECK(std::arg(goertzel(vec, line_b)) == Approx(math::pi / 2));
	CHECK(std::arg(goertzel(vec, line_c)) == Approx(math::pi / 4));
}

#endif
