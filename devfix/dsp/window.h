//
// Created by Praktikum on 06.05.2020.
//

#pragma once

namespace window
{
	template<std::size_t N>
	static constexpr double hanning(std::size_t k)
	{
		return k < N ? 0.5 + 0.5 * std::cos(2 * M_PI * (static_cast<signed>(k) - N / 2) / N) : 0;
	}

};

