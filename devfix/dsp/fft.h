//
// Created by core on 6/1/20.
//

#pragma once

#include <complex>
#include <cmath>
#include <cstring>
#include "../base/math.h"
#include "window.h"

namespace devfix::dsp
{
	struct fft
	{
		using math = devfix::base::math;

		template<typename FloatT>
		static void transform_inplace(std::complex<FloatT>* win, std::size_t len)
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
						std::complex<FloatT> t = win[a] - win[b];
						win[a] += win[b];
						win[b] = t * w;
					}
					w *= phi;
				}
			}

			for (std::size_t a = 1; a < len; a++)
			{
				std::size_t b = math::reverse_bits(a, math::popcount(len));
				if (b > a)
				{
					std::complex<FloatT> t = win[a];
					win[a] = win[b];
					win[b] = t;
				}
			}
		}

		template<typename FloatT>
		static void transform_inplace(std::vector<std::complex<FloatT>>& win)
		{
			transform_inplace<FloatT>(win.data(), win.size());
		}

		template<typename FloatT, std::size_t N>
		static void transform_inplace(std::array<std::complex<FloatT>, N>& win)
		{
			static_assert(N == 1 << math::popcount(N), "length is not a power of 2");  // assert N is power of 2
			transform_inplace<FloatT>(win.data(), win.size());
		}

		template<typename FloatT, FloatT(* win_fun)(std::size_t, std::size_t), std::size_t N>
		static void apply_window(std::complex<FloatT>* win)
		{
			for (std::size_t i = 0; i < N; i++) { win[i] *= window::get_window<FloatT, win_fun>(N, i); }
		}

		template<typename FloatT, FloatT(* win_fun)(std::size_t, std::size_t), std::size_t N>
		static void apply_window(std::vector<std::complex<FloatT>>& win)
		{
			if (win.size() != N) { throw std::invalid_argument("invalid vector size"); }
			apply_window<FloatT, win_fun, N>(win.data());
		}

		template<typename FloatT, FloatT(* win_fun)(std::size_t, std::size_t), std::size_t N>
		static void apply_window(std::array<std::complex<FloatT>, N>& win)
		{
			apply_window<FloatT, win_fun, N>(win.data());
		}

		template<typename FloatT>
		static void normalize_amplitude(const std::complex<FloatT>* win, std::size_t len, FloatT* mag)
		{
			mag[0] = std::abs<FloatT>(win[0] * (1. / len));
			for (std::size_t i = 1; i < len / 2; i++) { mag[i] = std::abs<FloatT>(win[i] * (2. / len)); }
			mag[len / 2] = std::abs<FloatT>(win[len / 2] * (1. / len));
		}

		template<typename FloatT>
		static std::vector<FloatT> normalize_amplitude(const std::vector<std::complex<FloatT>>& win)
		{
			std::vector<FloatT> mag(win.size() / 2 + 1);
			normalize_amplitude(win.data(), win.size(), mag.data());
			return mag;
		}

		template<typename FloatT, std::size_t N>
		static std::array<FloatT, N> normalize_amplitude(const std::array<std::complex<FloatT>, N>& win)
		{
			std::array<FloatT, N> mag(win.size() / 2 + 1);
			normalize_amplitude(win.data(), win.size(), mag.data());
			return mag;
		}
	};
}