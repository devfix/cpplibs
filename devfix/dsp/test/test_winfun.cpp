//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch2/catch.hpp>
#include "../../../testutil.h"
#include "../window.h"
#include "../winfun.h"

static constexpr double PRECISION_FINE = 1e-6;

using namespace devfix::base;
using namespace devfix::dsp;

template<typename FloatT>
std::vector<FloatT> get_win_vals(const window<FloatT>& win)
{
	std::vector<FloatT> vec(win.size(), FloatT(1));
	win.apply(vec);
	return vec;
}

TEST_CASE("devfix/dsp/winfun/hanning")
{
	using FloatT = float;
	std::vector<FloatT> expected = {
		0, 0.188255099070633, 0.611260466978157, 0.950484433951210, 0.950484433951210, 0.611260466978157, 0.188255099070633, 0
	};
	window<FloatT> win(winfun<FloatT>::hanning, 8, false);
	auto vals = get_win_vals(win);
	testutil::check_equals(vals, expected, testutil::MARGIN_FINE);
}

#endif
