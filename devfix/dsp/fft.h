//
// Created by core on 6/1/20.
//

#pragma once

#include <complex>
#include <cmath>
#include "../base/math.h"

namespace devfix::dsp::fft
{
	template<typename T>
	void basic_transform_inplace(std::complex<T>* vec, std::size_t len)
	{
		double theta = M_PI / len;
		std::complex<T> phi = std::complex<T>(cos(theta), -sin(theta)), w;
		for (std::size_t stage = len; stage > 1; stage >>= 1)
		{
			phi *= phi;
			w = 1;
			for (std::size_t pair = 0; pair < stage / 2; pair++)
			{
				for (std::size_t a = pair; a < len; a += stage)
				{
					std::size_t b = a + stage / 2;
					std::complex<T> t = vec[a] - vec[b];
					vec[a] += vec[b];
					vec[b] = t * w;
				}
				w *= phi;
			}
		}

		for (std::size_t a = 1; a < len; a++)
		{
			std::size_t b = devfix::base::math::reverse_bits(a, std::log2(len));
			if (b > a)
			{
				std::complex<T> t = vec[a];
				vec[a] = vec[b];
				vec[b] = t;
			}
		}
	}
}