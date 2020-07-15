//
// Created by core on 6/1/20.
//

#pragma once

#include <complex>
#include <cmath>
#include <vector>
#include <array>
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
			if ((1u << math::floorLog2(len)) != len) { throw std::invalid_argument("fft length is not a power of two"); }

			double theta = math::pi / len;
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

			// normalize fft
			FloatT inv_len = FloatT(1) / len;
			for (std::size_t i = 0; i < len; i++) { win[i] *= inv_len; }
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

		template<typename FloatT, window::win_fun_t<FloatT> win_fun>
		static void apply_window(std::complex<FloatT>* win, std::size_t n)
		{
			FloatT gain = window::calc_amplitude_gain<FloatT, win_fun>(n);
			for (std::size_t i = 0; i < n; i++) { win[i] *= gain * win_fun(n, i); }
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

		template<typename FloatT>
		static void convert_to_onesided(const std::complex<FloatT>* win, std::size_t len, std::complex<FloatT>* positive)
		{
			positive[0] = win[0];
			for (std::size_t i = 1; i < len / 2; i++) { positive[i] = FloatT(2) * win[i]; }
		}

		template<typename FloatT>
		static std::vector<std::complex<FloatT>> convert_to_onesided(const std::vector<std::complex<FloatT>>& win)
		{
			std::vector<std::complex<FloatT>> positive(win.size() / 2);
			convert_to_onesided(win.data(), win.size(), positive.data());
			return positive;
		}

		template<typename FloatT, std::size_t N>
		static std::array<std::complex<FloatT>, N / 2> convert_to_onesided(const std::array<std::complex<FloatT>, N>& win)
		{
			std::array<std::complex<FloatT>, N / 2> positive;
			convert_to_onesided(win.data(), win.size(), positive.data());
			return positive;
		}

		/**
		 * \brief calculates the angles (phase) in radians for each complex number
		 * \tparam FloatT floating point type
		 * \param win field of complex numbers to extract the angles from
		 * \param len length of field
		 * \param angles target field to store the angles in, should have at least same size as win
		 * \param threshold if set to a positive value, the angles is set to zero if the abs value of the complex number is below this threshold
		 */
		template<typename FloatT>
		static void extract_angles(const std::complex<FloatT>* win, std::size_t len, FloatT* angles, FloatT threshold = -1)
		{
			if (threshold < 0)
			{
				for (std::size_t i = 1; i < len; i++) { angles[i] = std::arg(win[i]); }
			}
			else
			{
				for (std::size_t i = 1; i < len; i++)
				{
					if (std::abs(win[i]) > threshold) { angles[i] = std::arg(win[i]); }
					else { angles[i] = 0; }
				}
			}
		}

		/**
		 * \brief calculates the angles (phase) in radians for each complex number
		 * \tparam FloatT floating point type
		 * \param win vector of complex numbers to extract the angles from
		 * \param threshold if set to a positive value, the angles is set to zero if the abs value of the complex number is below this threshold
		 * \return vector of extracted angles, same length as win
		 */
		template<typename FloatT>
		static std::vector<FloatT> extract_angles(const std::vector<std::complex<FloatT>>& win, FloatT threshold = -1)
		{
			std::vector<FloatT> angles(win.size());
			extract_angles(win.data(), win.size(), angles.data(), threshold);
			return angles;
		}

		/**
		 * \brief calculates the angles (phase) in radians for each complex number
		 * \tparam FloatT floating point type
		 * \tparam N length of array
		 * \param win array of complex numbers to extract the angles from
		 * \param threshold if set to a positive value, the angles is set to zero if the abs value of the complex number is below this threshold
		 * \return array of extracted angles, same length as win
		 */
		template<typename FloatT, std::size_t N>
		static std::array<FloatT, N> extract_angles(const std::array<std::complex<FloatT>, N>& win, FloatT threshold = -1)
		{
			std::array<FloatT, N> angles(win.size());
			extract_angles(win.data(), win.size(), angles.data(), threshold);
			return angles;
		}
	};
}