//
// Created by core on 7/15/20.
//

#pragma once

#include <complex>
#include <vector>
#include <array>

#include "../base/math.h"

using math = devfix::base::math;

namespace devfix::dsp
{

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::complex<FloatT>* field, std::size_t len, std::size_t k)
	{
		const auto wn = std::polar(FloatT(1), FloatT(math::pi * 2. * k / len));
		std::complex<FloatT> s = field[0];
		for (std::int32_t i = 1; i < len; i++)
		{
			s *= wn;
			s += field[i];
		}
		return s * wn * (1. / len);
	}

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::vector<std::complex<FloatT>>& vec, std::size_t k)
	{
		return goertzel(vec.data(), vec.size(), k);
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::array<std::complex<FloatT>, N>& arr, std::size_t k)
	{
		return goertzel(arr.data(), arr.size(), k);
	}

}
