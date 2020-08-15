//
// Created by core on 6/1/20.
//

#pragma once

#include <complex>
#include <cmath>
#include <vector>
#include <array>
#include "../base/math.h"
#include "../base/numbers.h"

namespace devfix::dsp
{
	struct fft
	{
		using math  = ::devfix::base::math;

		///////////////////////
		// transform_inplace //
		///////////////////////

		template<typename FloatT>
		static void transform_inplace(std::complex<FloatT>* win, std::size_t len)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			namespace numbers = devfix::base::numbers;
			if (math::exp2(math::floorLog2(len)) != len) { throw std::invalid_argument("len is not a power of 2"); }

			double theta = numbers::pi_v<FloatT> / len;
			std::complex<FloatT> phi = std::complex<FloatT>(cos(theta), -sin(theta)), w;
			for (std::size_t stage = len; stage > 1; stage >>= 1u)
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
			static_assert(std::is_floating_point_v<FloatT>);
			transform_inplace<FloatT>(win.data(), win.size());
		}

		template<typename FloatT, std::size_t N>
		static void transform_inplace(std::array<std::complex<FloatT>, N>& win)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			static_assert(N == math::exp2(math::floorLog2(N)), "array size is not a power of 2");  // assert N is power of 2
			transform_inplace<FloatT>(win.data(), win.size());
		}

		///////////////////////
		// normalize inplace //
		///////////////////////

		template<typename FloatT>
		static void normalize_inplace(std::complex<FloatT>* win, std::size_t len, std::size_t n = 0)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (n == 0) { n = len; }
			const FloatT inv_n = FloatT(1) / n;
			for (std::size_t i = 0; i < len; i++) { win[i] *= inv_n; }
		}

		template<typename FloatT>
		static void normalize_inplace(std::vector<std::complex<FloatT>>& win, std::size_t n = 0)
		{
			normalize_inplace<FloatT>(win.data(), win.size(), n);
		}

		template<typename FloatT, std::size_t N>
		static void normalize_inplace(std::array<std::complex<FloatT>, N>& win, std::size_t n = 0)
		{
			normalize_inplace<FloatT>(win.data(), win.size(), n);
		}

		///////////////////
		// get magnitude //
		///////////////////

		template<typename FloatT>
		static void get_magnitude(const std::complex<FloatT>* win, std::size_t len, FloatT* mag)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (len == 0) { throw std::invalid_argument("len is not greater zero"); }
			mag[0] = std::abs(win[0]);
			for (std::size_t i = 1; i < len / 2; i++) { mag[i] = std::abs(win[i]) * FloatT(2); }
		}

		template<typename FloatT>
		static std::vector<FloatT> get_magnitude(const std::vector<std::complex<FloatT>>& win)
		{
			std::vector<FloatT> mag(win.size() / 2);
			get_magnitude<FloatT>(win.data(), win.size(), mag.data());
			return mag;
		}

		template<typename FloatT, std::size_t N>
		static std::array<FloatT, N / 2> get_magnitude(const std::array<std::complex<FloatT>, N>& win)
		{
			static_assert(N > 0, "len is not greater zero");
			std::array<FloatT, N / 2> mag;
			get_magnitude<FloatT>(win.data(), win.size(), mag.data());
			return mag;
		}

		///////////////
		// get phase //
		///////////////

		template<typename FloatT>
		static void get_phase(const std::complex<FloatT>* win, std::size_t len, FloatT* ph)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (len == 0) { throw std::invalid_argument("len is not greater zero"); }
			ph[0] = std::arg(win[0]);
			for (std::size_t i = 1; i < len; i++) { ph[i] = std::arg(win[i]); }
		}

		template<typename FloatT>
		static std::vector<FloatT> get_phase(const std::vector<std::complex<FloatT>>& win)
		{
			std::vector<FloatT> ph(win.size());
			get_phase<FloatT>(win.data(), win.size(), ph.data());
			return ph;
		}

		template<typename FloatT, std::size_t N>
		static std::array<FloatT, N / 2> get_phase(const std::array<std::complex<FloatT>, N>& win)
		{
			static_assert(N > 0, "len is not greater zero");
			std::array<FloatT, win.size()> ph;
			get_phase<FloatT>(win.data(), win.size(), ph.data());
			return ph;
		}
	};
}
