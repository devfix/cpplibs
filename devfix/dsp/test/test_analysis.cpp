//
// Created by core on 5/25/20.
//

#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch.hpp>
#include "../analysis.h"
#include "../window.h"

static constexpr double PRECISION_FINE = 1e-6;

using namespace devfix::base;
using namespace devfix::dsp;

TEST_CASE("Analysis - RMS")
{
	std::vector<std::complex<double>> spec = {{ 1, 0 }, { 0, math::sqrt2 }, { -math::sqrt2, 0 }, { 0, -math::sqrt2 }};
	auto rms = analysis::rms(spec);
	REQUIRE(rms.size() == spec.size());
	CHECK(rms[0] == Approx(1).margin(PRECISION_FINE));
	CHECK(rms[1] == Approx(1).margin(PRECISION_FINE));
	CHECK(rms[2] == Approx(1).margin(PRECISION_FINE));
	CHECK(rms[3] == Approx(1).margin(PRECISION_FINE));
}

TEST_CASE("Analysis - AmplTHDN")
{
	{
		std::vector<double> rms = { 2, 0, 0, 4, 0, 1, 0, 2 };
		CHECK(analysis::ampl_thdn(rms.data(), rms.size(), { 5 }) == Approx(4.898979486).margin(PRECISION_FINE));
		CHECK(analysis::ampl_thdn(rms.data(), rms.size(), { 0 }) == Approx(2.291287847).margin(PRECISION_FINE));
	}
	{
		std::vector<double> rms = { 1, 0, 0.1, 0.2, 0, 0.05, 0, 0.09 };
		CHECK(analysis::ampl_thdn(rms.data(), rms.size(), { 0 }) == Approx(0.246170673).margin(PRECISION_FINE));
		CHECK(analysis::ampl_thdn(rms.data(), rms.size(), { 3 }) == Approx(5.051237472).margin(PRECISION_FINE));
	}
}

TEST_CASE("Analysis - AmplTHDN with fft")
{
	constexpr std::size_t FFT_LEN = 128;
	std::vector<std::complex<double>> vec(FFT_LEN);
	for (std::size_t i = 0; i < FFT_LEN; i++)
	{
		vec[i] = 0.05 * std::sin(2. * math::pi * 8. * double(i) / double(FFT_LEN))
			+ 0.5 * std::sin(2. * math::pi * 2. * double(i) / double(FFT_LEN))
			- 0.001;
	}

	CHECK((analysis::ampl_thdn<double, window::rectangle>(vec, { 0 }) == Approx(355.31676008879958).margin(PRECISION_FINE)));
	CHECK((analysis::ampl_thdn<double, window::rectangle>(vec, { 2 }) == Approx(0.100039992).margin(PRECISION_FINE)));
}

#endif
