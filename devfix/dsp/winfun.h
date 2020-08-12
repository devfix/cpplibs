//
// Created by core on 8/12/20.
//

#pragma once

#include <cmath>

#include "../base/math.h"

namespace devfix::dsp
{
	template <typename FloatT>
	struct winfun
	{

		static constexpr FloatT hanning(std::size_t n, std::size_t k)
		{
			return k < n ? (
				FloatT(.5) + FloatT(.5) * std::cos(FloatT(2) * base::math::pi * (FloatT(k) - FloatT(.5) * FloatT(n - 1)) / (n - 1))
			) : 0;
		}

	};
}