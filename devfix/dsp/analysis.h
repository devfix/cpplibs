//
// Created by core on 7/6/20.
//

#pragma once

#include <complex>
#include <array>
#include <vector>
#include "fft.h"
#include "dsp.h"
#include "../base/math.h"

namespace devfix::dsp
{
	struct analysis
	{
		template<typename FloatT>
		static constexpr void rms(const std::complex<FloatT>* field, std::size_t len, FloatT* res)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			res[0] = std::abs(field[0]);  // dc line is already the rms
			for (std::size_t i = 1; i < len; i++) { res[i] = math::sqrt1_2 * std::abs(field[i]); }  // calc sine rms
		}

		template<typename FloatT, std::size_t N>
		static constexpr std::array<FloatT, N> rms(const std::array<std::complex<FloatT>, N>& arr)
		{
			std::array<FloatT, N> res;
			rms<FloatT, N>(arr.data(), N, res.data());
			return res;
		}

		template<typename FloatT>
		static constexpr std::vector<FloatT> rms(const std::vector<std::complex<FloatT>>& vec)
		{
			std::vector<FloatT> res(vec.size());
			rms<FloatT>(vec.data(), vec.size(), res.data());
			return res;
		}

		template<typename FloatT>
		static constexpr FloatT ampl_thdn(const FloatT* rms, std::size_t len, const std::vector<std::size_t>& sig_lines)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			FloatT sum_total = 0;
			for (std::size_t i = 0; i < len; i++) { sum_total += rms[i] * rms[i]; }
			FloatT sum_sig = 0;
			for (auto& l : sig_lines) { sum_sig += rms[l] * rms[l]; }
			return std::sqrt(sum_total - sum_sig) / std::sqrt(sum_sig);
		}

		template<typename FloatT, window::win_fun_t<FloatT> win_fun>
		static constexpr FloatT ampl_thdn(std::vector<std::complex<FloatT>> vec, const std::vector<std::size_t>& sig_lines)
		{
			apply_window<FloatT, win_fun>(vec);
			fft::transform_inplace(vec);
			const auto onesided = fft::convert_to_onesided(vec);
			const auto rms_vec = rms(onesided);
			return ampl_thdn(rms_vec.data(), rms_vec.size(), sig_lines);
		}
	};

} // namespace devfix::dsp
