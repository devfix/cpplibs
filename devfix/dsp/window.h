//
// Created by core on 06.05.2020.
//

#pragma once

#include <cmath>
#include <numeric>
#include <map>

#include "../base/math.h"

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
		using math = devfix::base::math;

		template<typename FloatT>
		using win_fun_t = FloatT(*)(std::size_t, std::size_t);

		template<typename FloatT, win_fun_t<FloatT> win_fun>
		struct buffer
		{
		private:
			static inline std::map<std::size_t, std::vector<FloatT>> _coeffs_buffer;

		public:
			static const std::vector<FloatT>& prepare(std::size_t n)
			{
				// check if coefficients for window of size n are not already calculated
				if (auto it = _coeffs_buffer.find(n); it != _coeffs_buffer.end())
				{
					return it->second;
				}
				else
				{
					FloatT gain = calc_amplitude_gain<FloatT, win_fun>(n);
					auto& vec = _coeffs_buffer.emplace(n, n).first->second;
					for (std::size_t k = 0; k < n; k++) { vec[k] = gain * win_fun(n, k); }
					return vec;
				}
			}

			static void clear() { _coeffs_buffer.clear(); }

			static std::size_t size()
			{
				return std::accumulate(_coeffs_buffer.begin(), _coeffs_buffer.end(), 0l,
									   [](const std::size_t prev, const auto& pair) { return prev + pair.second.size(); });
			}

			static FloatT get_window(std::size_t n, std::size_t k) { return prepare(n)[k]; }

		};

		template<typename FloatT, win_fun_t<FloatT> win_fun>
		static constexpr FloatT calc_amplitude_gain(std::size_t n)
		{
			FloatT sum = 0;
			for (std::size_t i = 0; i < n; i++) { sum += win_fun(n, i); }
			return FloatT(n) / sum;
		}

		template<typename FloatT>
		static constexpr FloatT hanning(std::size_t n, std::size_t k)
		{
			return k < n ? (FloatT(.5) + FloatT(.5) * std::cos(FloatT(2) * math::pi * (FloatT(k) - FloatT(.5) * FloatT(n - 1)) / (n - 1))) : 0;
		}

		template<typename FloatT>
		static constexpr FloatT flattop(std::size_t n, std::size_t k)
		{
			return k < n ? (
				FloatT(FLATTOP_COEFFICIENT_A0)
					- FloatT(FLATTOP_COEFFICIENT_A1) * std::cos(FloatT(2.) * math::pi * k / (n - 1))
					+ FloatT(FLATTOP_COEFFICIENT_A2) * std::cos(FloatT(4.) * math::pi * k / (n - 1))
					- FloatT(FLATTOP_COEFFICIENT_A3) * std::cos(FloatT(6.) * math::pi * k / (n - 1))
					+ FloatT(FLATTOP_COEFFICIENT_A4) * std::cos(FloatT(8.) * math::pi * k / (n - 1))
			) : 0;
		}

		template<typename FloatT>
		static constexpr FloatT rectangle(std::size_t n, std::size_t k) { return k < n ? 1 : 0; }

		// linear window, only for testing purposes
		template<typename FloatT>
		static constexpr FloatT linear(std::size_t n, std::size_t k)
		{
			return k < n ? double(k) / double(n - 1) : 0;
		}
	};

}

