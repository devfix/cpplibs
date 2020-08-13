//
// Created by core on 8/12/20.
//

#pragma once

#include <cmath>

#include "../base/math.h"

namespace devfix::dsp
{
	template<typename FloatT>
	struct winfun
	{
		static constexpr FloatT hanning(std::size_t n, std::size_t k)
		{
			return k < n ? FloatT
				(FloatT(.5) + FloatT(.5) * std::cos(FloatT(2) * base::math::pi * (FloatT(k) - FloatT(.5) * (FloatT(n) - 1)) / (n - 1)))
						 : FloatT(0);
		}

		static constexpr FloatT hamming(std::size_t n, std::size_t k)
		{
			return get_cosine_win(n, k, { 0.54, -(1 - 0.54) });
		}

		static constexpr FloatT flattop_matlab(std::size_t n, std::size_t k)
		{
			return get_cosine_win(n, k, { 0.21557895, -0.41663158, 0.277263158, -0.083578947, 0.006947368 });
		}

	private:
		static FloatT get_cosine_win(std::size_t n, std::size_t k, std::vector<FloatT> coeffs)
		{
			if (k < n)
			{
				FloatT sum = 0;
				for (std::size_t i = 0; i < coeffs.size(); i++)
				{
					sum += coeffs[i] * std::cos(FloatT(2 * base::math::pi * i * k) / (n - 1));
				}
				return sum;
			}
			return 0;
		}

	};
}