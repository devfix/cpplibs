//
// Created by core on 6/1/20.
//

#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "addr.h"
#include "../base/math.h"

namespace devfix::dsp::fft
{
	template<std::size_t N, typename T>
	void transform_inplace(std::complex<T>* vec)
	{
		double theta = M_PI / N;
		std::complex<T> phi = std::complex<T>(cos(theta), -sin(theta)), w;
		for (std::size_t stage = N; stage > 1; stage >>= 1)
		{
			phi *= phi;
			w = 1;
			for (std::size_t pair = 0; pair < stage / 2; pair++)
			{
				for (std::size_t a = pair; a < N; a += stage)
				{
					std::size_t b = a + stage / 2;
					std::complex<T> t = vec[a] - vec[b];
					vec[a] += vec[b];
					vec[b] = t * w;
				}
				w *= phi;
			}
		}

		for (std::size_t a = 1; a < N; a++)
		{
			std::size_t b = devfix::base::math::reverse_bits(a, devfix::base::math::log2<N>::value);
			if (b > a)
			{
				std::complex<T> t = vec[a];
				vec[a] = vec[b];
				vec[b] = t;
			}
		}
	}
}