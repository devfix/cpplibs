//
// Created by core on 06.05.2020.
//

#pragma once
#include <cmath>

namespace devfix::dsp
{

	struct window
	{
	private:
		static constexpr double FLATTOP_COEFFICIENT_A0 = 0.21557895;
		static constexpr double FLATTOP_COEFFICIENT_A1 = 0.41663158;
		static constexpr double FLATTOP_COEFFICIENT_A2 = 0.277263158;
		static constexpr double FLATTOP_COEFFICIENT_A3 = 0.083578947;
		static constexpr double FLATTOP_COEFFICIENT_A4 = 0.006947368;

	public:
		template<typename FloatT, FloatT(* win_fun)(std::size_t, std::size_t)>
		static constexpr FloatT calc_amplitude_gain(std::size_t n)
		{
			FloatT sum = 0;
			for (std::size_t i = 0; i < n; i++) { sum += win_fun(n, i); }
			return FloatT(n) / sum;
		}

		template<typename FloatT, FloatT(* win_fun)(std::size_t, std::size_t)>
		static constexpr FloatT get_window(std::size_t n, std::size_t k)
		{
			auto gain = calc_amplitude_gain<FloatT, win_fun>(n);
			return win_fun(n, k) * gain;
		}

		template<typename FloatT>
		static constexpr FloatT hanning(std::size_t N, std::size_t k)
		{
			return k < N ? (FloatT(.5) + FloatT(.5) * std::cos(FloatT(2) * M_PI * (FloatT(k) - FloatT(.5) * FloatT(N - 1)) / (N - 1))) : 0;
		}

		template<typename FloatT>
		static constexpr FloatT flattop(std::size_t n, std::size_t k)
		{
			return k < n ? (
				FloatT(FLATTOP_COEFFICIENT_A0)
					- FloatT(FLATTOP_COEFFICIENT_A1) * std::cos(FloatT(2.) * M_PI * k / (n - 1))
					+ FloatT(FLATTOP_COEFFICIENT_A2) * std::cos(FloatT(4.) * M_PI * k / (n - 1))
					- FloatT(FLATTOP_COEFFICIENT_A3) * std::cos(FloatT(6.) * M_PI * k / (n - 1))
					+ FloatT(FLATTOP_COEFFICIENT_A4) * std::cos(FloatT(8.) * M_PI * k / (n - 1))
			) : 0;
		}

		template<typename FloatT>
		static constexpr FloatT rectangle(std::size_t n, std::size_t k) { return k < n ? 1 : 0; }
	};

}

