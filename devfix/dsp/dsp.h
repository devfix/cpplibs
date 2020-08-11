//
// Created by core on 7/15/20.
//

#pragma once

#include <complex>
#include <vector>
#include <array>

#include "window.h"
#include "../base/math.h"

using math = devfix::base::math;

namespace devfix::dsp
{

	/**
	 * \brief calculates an individual term of the discrete Fourier transform
	 * \tparam FloatT floating point type
	 * \param field
	 * \param len
	 * \param k
	 * \return
	 */
	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::complex<FloatT>* field, std::size_t len, std::size_t k)
	{
		static_assert(std::is_floating_point_v<FloatT>);
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

	template<typename FloatT, window::win_fun_t<FloatT> win_fun>
	static void apply_window(std::complex<FloatT>* win, std::size_t n)
	{
		FloatT gain = window::calc_amplitude_gain<FloatT, win_fun>(n);
		for (std::size_t i = 0; i < n; i++) {
			win[i] *= gain; //  win_fun(n, i)
		}
	}

	template<typename FloatT, window::win_fun_t<FloatT> win_fun>
	static void apply_window(std::vector<std::complex<FloatT>>& win)
	{
		apply_window<FloatT, win_fun>(win.data(), win.size());
	}

	template<typename FloatT, window::win_fun_t<FloatT> win_fun, std::size_t N>
	static void apply_window(std::array<std::complex<FloatT>, N>& win)
	{
		apply_window<FloatT, win_fun>(win.data(), win.size());
	}

	template<typename FloatT>
	static void apply_window(std::complex<FloatT>* win, window::win_fun_t<FloatT> win_fun, std::size_t n)
	{
		for (std::size_t i = 0; i < n; i++) { win[i] *= win_fun(n, i); }
	}

	template<typename FloatT>
	static void apply_window(std::vector<std::complex<FloatT>>& win, window::win_fun_t<FloatT> win_fun)
	{
		apply_window<FloatT>(win.data(), win_fun, win.size());
	}

	template<typename FloatT, std::size_t N>
	static void apply_window(std::array<std::complex<FloatT>, N>& win, window::win_fun_t<FloatT> win_fun)
	{
		apply_window<FloatT>(win.data(), win_fun, win.size());
	}

}
