//
// Created by core on 8/12/20.
//

#pragma once

#include <cstddef>

namespace devfix::dsp
{
	/**
	 * \brief type for window functions
	 * @param n window size
	 * @param k window element index
	 */
	template<typename FloatT>
	using winfun_t = FloatT(*)(std::size_t n, std::size_t k);
}
