//
// Created by core on 6/1/20.
//

#pragma once

#include <complex>
#include <cmath>
#include "../base/math.h"

namespace devfix::dsp
{
	struct fft
	{
		using math = devfix::base::math;

		template<typename FloatT>
		static void transform_inplace(std::complex<FloatT>* field, std::size_t len)
		{
			double theta = M_PI / len;
			std::complex<FloatT> phi = std::complex<FloatT>(cos(theta), -sin(theta)), w;
			for (std::size_t stage = len; stage > 1; stage >>= 1)
			{
				phi *= phi;
				w = 1;
				for (std::size_t pair = 0; pair < stage / 2; pair++)
				{
					for (std::size_t a = pair; a < len; a += stage)
					{
						std::size_t b = a + stage / 2;
						std::complex<FloatT> t = field[a] - field[b];
						field[a] += field[b];
						field[b] = t * w;
					}
					w *= phi;
				}
			}

			for (std::size_t a = 1; a < len; a++)
			{
				std::size_t b = math::reverse_bits(a, math::popcount(len));
				if (b > a)
				{
					std::complex<FloatT> t = field[a];
					field[a] = field[b];
					field[b] = t;
				}
			}
		}

		template<typename FloatT>
		static void transform_inplace(std::vector<std::complex<FloatT>>& vec)
		{
			transform_inplace<FloatT>(vec.data(), vec.size());
		}

		template<typename FloatT, std::size_t N>
		static void transform_inplace(std::array<std::complex<FloatT>, N>& arr)
		{
			static_assert(N == 1 << math::popcount(N), "length is not a power of 2");  // assert N is power of 2
			transform_inplace<FloatT>(arr.data(), arr.size());
		}
	};
}