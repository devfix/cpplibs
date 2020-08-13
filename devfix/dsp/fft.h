//
// Created by core on 6/1/20.
//

#pragma once

#include <complex>
#include <cmath>
#include <vector>
#include <array>
#include <tuple>
#include "../base/math.h"

namespace devfix::dsp
{
	struct fft
	{

		using math = devfix::base::math;

		template<typename FloatT>
		static void transform_inplace(std::complex<FloatT>* win, std::size_t len)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (math::exp2(math::floorLog2(len)) != len) { throw std::invalid_argument("len is not a power of 2"); }

			double theta = math::pi / len;
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

		template<typename FloatT>
		static void get_mag_and_phase(const std::complex<FloatT>* win, std::size_t len, std::pair<FloatT, FloatT>* magphase)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (len == 0) { throw std::invalid_argument("len is not greater zero"); }
			const FloatT inv_n = FloatT(1) / len;
			magphase->first = std::abs(win[0] * inv_n);
			magphase->second = std::arg(win[0] * inv_n);
			for (std::size_t i = 1; i < len / 2; i++)
			{
				const auto dft = win[i] * inv_n * 2.;
				magphase[i].first = std::abs(dft);
				magphase[i].second = std::arg(dft);
			}
		}

		template<typename FloatT>
		static std::vector<std::pair<FloatT, FloatT>> get_mag_and_phase(const std::vector<std::complex<FloatT>>& win)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			std::vector<std::pair<FloatT, FloatT>> magphase(win.size() / 2);
			get_mag_and_phase(win.data(), win.size(), magphase.data());
			return magphase;
		}

		template<typename FloatT, std::size_t N>
		static std::array<std::pair<FloatT, FloatT>, N / 2> get_mag_and_phase(const std::array<std::complex<FloatT>, N>& win)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			static_assert(N > 0, "len is not greater zero");
			std::array<std::pair<FloatT, FloatT>, N / 2> magphase;
			get_mag_and_phase(win.data(), win.size(), magphase.data());
			return magphase;
		}
	};
}
