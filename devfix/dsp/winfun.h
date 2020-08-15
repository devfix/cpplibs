//
// Created by core on 8/12/20.
//

#pragma once

#include <cmath>

#include "../base/math.h"
#include "../base/numbers.h"

namespace devfix::dsp
{
	struct winfun
	{

		template<typename FloatT>
		static constexpr FloatT hanning(std::size_t n, std::size_t k)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			namespace numbers = devfix::base::numbers;
			return k < n ? FloatT
				(FloatT(.5) + FloatT(.5) * std::cos(FloatT(2) * numbers::pi_v<FloatT> * (FloatT(k) - FloatT(.5) * (FloatT(n) - 1)) / (n - 1)))
						 : FloatT(0);
		}

		template<typename FloatT>
		static constexpr FloatT hamming(std::size_t n, std::size_t k)
		{
			return get_cosine_win<FloatT>(n, k, { 0.54, -(1 - 0.54) });
		}

		template<typename FloatT>
		static constexpr FloatT flattop_matlab(std::size_t n, std::size_t k)
		{
			return get_cosine_win<FloatT>(n, k, { 0.21557895, -0.41663158, 0.277263158, -0.083578947, 0.006947368 });
		}

		template<typename FloatT>
		static constexpr FloatT flattop_hft248d(std::size_t n, std::size_t k)
		{
			return get_cosine_win<FloatT>(n, k, {
				1, -1.985844164102, 1.791176438506, -1.282075284005, 0.667777530266, -0.240160796576, 0.056656381764, -0.008134974479,
				0.000624544650, -0.000019808998, 0.000000132974,
			});
		}

	private:
		template<typename FloatT>
		static constexpr FloatT get_cosine_win(std::size_t n, std::size_t k, const std::vector<FloatT>& coeffs)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			namespace numbers = devfix::base::numbers;
			if (k < n)
			{
				FloatT sum = 0;
				for (std::size_t i = 0; i < coeffs.size(); i++)
				{
					sum += coeffs[i] * std::cos(FloatT(2 * numbers::pi_v<FloatT> * i * k) / (n - 1));
				}
				return sum;
			}
			return 0;
		}

	};
}