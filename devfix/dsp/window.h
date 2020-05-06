//
// Created by core on 06.05.2020.
//

#pragma once

namespace window
{
	template<std::size_t N>
	static constexpr double hanning(int k)
	{ return k < N ? .5 + .5 * std::cos(2 * M_PI * (static_cast<signed>(k) - .5 * double(N - 1)) / (N - 1)) : 0; }
};

