//
// Created by core on 06.05.2020.
//

#pragma once
#include <cmath>

namespace devfix::dsp
{

	namespace window
	{
		constexpr double HANNING_FACTOR = 2;

		static constexpr double hanning(std::size_t N, std::size_t k)
		{
			return k < N ? .5 + .5 * std::cos(2 * M_PI * (double(k) - .5 * double(N - 1)) / (N - 1)) : 0;
		}

		static constexpr double FLATTOP_COEFFICIENT_A0 = 0.21557895;

		static constexpr double FLATTOP_COEFFICIENT_A1 = 0.41663158;

		static constexpr double FLATTOP_COEFFICIENT_A2 = 0.277263158;

		static constexpr double FLATTOP_COEFFICIENT_A3 = 0.083578947;

		static constexpr double FLATTOP_COEFFICIENT_A4 = 0.006947368;

		constexpr double FLATTOP_FACTOR = 1. / FLATTOP_COEFFICIENT_A0;

		static constexpr double flattop(std::size_t N, std::size_t k)
		{
			return k < N ? (
				FLATTOP_COEFFICIENT_A0
					- FLATTOP_COEFFICIENT_A1 * std::cos(2. * M_PI * k / (N - 1))
					+ FLATTOP_COEFFICIENT_A2 * std::cos(4. * M_PI * k / (N - 1))
					- FLATTOP_COEFFICIENT_A3 * std::cos(6. * M_PI * k / (N - 1))
					+ FLATTOP_COEFFICIENT_A4 * std::cos(8. * M_PI * k / (N - 1))
			) : 0;
		}

	};

}

