//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch/catch.hpp>
#include "../dsp.h"
#include "../../base/math.h"

static constexpr double PRECISION_FINE = 1e-6;

using namespace devfix::base;
using namespace devfix::dsp;

TEST_CASE("DSP - Goertzel")
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
	CHECK(2 * std::abs(goertzel(vec)) == Approx(1. * 4).epsilon(PRECISION_FINE));
	CHECK(2 * std::abs(goertzel(vec)) == Approx(1. / 8).epsilon(PRECISION_FINE));
	CHECK(2 * std::abs(goertzel(vec)) == Approx(1. / 4).epsilon(PRECISION_FINE));
	CHECK(2 * std::abs(goertzel(vec)) == Approx(0).margin(PRECISION_FINE));
	CHECK(2 * std::abs(goertzel(vec)) == Approx(1. / 2).epsilon(PRECISION_FINE));
	//for (std::size_t i = 5; i < LEN / 2; i++) { REQUIRE(mag[i] == Approx(0).margin(PRECISION_FINE)); }
}

#endif
