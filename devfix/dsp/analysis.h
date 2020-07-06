//
// Created by core on 7/6/20.
//

#pragma once

#include <complex>
#include <array>
#include <vector>

namespace devfix::dsp
{

	struct analysis
	{
		template<typename FloatT>
		static void rms(const std::complex<FloatT>* field, std::size_t len, FloatT* res)
		{
			res[0] = std::abs(field[0]);  // dc line is already the rms
			for (std::size_t i = 1; i < len; i++) { res[i] = MATH_SQRT1_2 * std::abs(field[i]); }  // calc sine rms
		}

		template<typename FloatT, std::size_t N>
		static std::array<FloatT, N> rms(const std::array<std::complex<FloatT>, N>& arr)
		{
			std::array<FloatT, N> res;
			rms<FloatT, N>(arr.data(), N, res.data());
			return res;
		}

		template<typename FloatT>
		static std::vector<FloatT> rms(const std::vector<std::complex<FloatT>>& vec)
		{
			std::vector<FloatT> res(vec.size());
			rms<FloatT>(vec.data(), vec.size(), res.data());
			return res;
		}

		template<typename FloatT>
		static FloatT ampl_thdn(const FloatT* rms, std::size_t len, FloatT sig_rms)
		{
			FloatT sum = 0;
			for (std::size_t i = 0; i < len; i++) { sum += rms[i] * rms[i]; }
			sum -= sig_rms * sig_rms;
			return std::sqrt(sum) / sig_rms;
		}

	private:
		static constexpr double MATH_SQRT1_2 = 0.70710678118654752440;
	};

} // namespace devfix::dsp
