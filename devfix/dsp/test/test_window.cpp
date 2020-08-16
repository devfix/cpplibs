//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <testutil.h>
#include "../window.h"
#include "../winfun.h"

using namespace devfix::base;
using namespace devfix::dsp;

template<typename FloatT>
std::vector<FloatT> get_win_vals(const window<FloatT>& win)
{
	std::vector<FloatT> vec(win.size(), FloatT(1));
	win.apply(vec);
	return vec;
}

TEST_CASE("devfix/dsp/window/gain")
{
	auto test_types = [] < typename FloatT > (FloatT
	_, long double
	margin)
	{
		auto test_winfun = [&](winfun_t<FloatT> winfun)
		{
			for (std::size_t n = 4; n <= (1u << 16u); n <<= 2u)
			{
				window<FloatT> win(winfun, n, true);
				auto vals = get_win_vals(win);
				auto gain = std::accumulate(vals.begin(), vals.end(), FloatT(0)) / vals.size();
				CHECK(gain == Approx(FloatT(1)).margin(margin));
			}
		};

		// register all window functions to be tested here
		test_winfun(winfun::hanning);
		test_winfun(winfun::flattop_matlab);
	};

	test_types((float) (1), testutil::MARGIN_COARSE);
	test_types((double) (1), testutil::MARGIN_FINE);
	test_types((long double) (1), testutil::MARGIN_FINE);
}


#endif
