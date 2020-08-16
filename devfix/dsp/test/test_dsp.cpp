//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <testutil.h>
#include "../dsp.h"

using namespace devfix::dsp;
namespace numbers = devfix::base::numbers;


TEST_CASE("devfix/dsp/dsp/calcsignal")
{
	constexpr std::size_t N = 1u << 16u;
	constexpr double fs = 44100;
	constexpr double niq = fs / 2 - 100;
	std::vector<double> vec(N);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		const double t = double(i) / fs;
		vec[i] = 1.2
			+ 1.0 * std::cos(2 * numbers::pi * 220 * t + 0.1)
			+ 0.8 * std::cos(2 * numbers::pi * 440 * t + 0.2)
			+ 0.6 * std::cos(2 * numbers::pi * 880 * t + 0.3)
			+ 0.4 * std::cos(2 * numbers::pi * niq * t + 0.4);
	}

	const auto dft220 = calcsignal(fs, 220., vec);
	const auto dft440 = calcsignal(fs, 440., vec);
	const auto dft880 = calcsignal(fs, 880., vec);
	const auto dftniq = calcsignal(fs, niq, vec);

	// test magnitudes
	CHECK(std::abs(dft220) == Approx(1.0).margin(testutil::MARGIN_COARSE));
	CHECK(std::abs(dft440) == Approx(0.8).margin(testutil::MARGIN_COARSE));
	CHECK(std::abs(dft880) == Approx(0.6).margin(testutil::MARGIN_COARSE));
	CHECK(std::abs(dftniq) == Approx(0.4).margin(testutil::MARGIN_COARSE));

	// test phases
	CHECK(std::arg(dft220) == Approx(0.1).margin(testutil::MARGIN_COARSE));
	CHECK(std::arg(dft440) == Approx(0.2).margin(testutil::MARGIN_COARSE));
	CHECK(std::arg(dft880) == Approx(0.3).margin(testutil::MARGIN_COARSE));
	CHECK(std::arg(dftniq) == Approx(0.4).margin(testutil::MARGIN_COARSE));
}

#endif
