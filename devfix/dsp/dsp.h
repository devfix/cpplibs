//
// Created by core on 7/15/20.
//

#pragma once

#include <complex>
#include <vector>
#include <array>

namespace devfix::dsp
{

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::complex<FloatT>* field, std::size_t len)
	{
		return 0;
	}

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::vector<std::complex<FloatT>>& vec)
	{
		return goertzel(vec.data(), vec.size());
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::array<std::complex<FloatT>, N>& arr)
	{
		return goertzel(arr.data(), arr.size());
	}

}
