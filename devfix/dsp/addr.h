//
// Created by core on 5/25/20.
//

#pragma once

#include "../base/meta/math.h"

namespace devfix::dsp
{
	template<std::size_t BITS, std::size_t I, std::size_t K>
	struct _bit_reverse;

	template<std::size_t BITS, std::size_t I>
	struct _bit_reverse<BITS, I, 0> { enum { value = 0 }; };

	template<std::size_t BITS, std::size_t I, std::size_t K>
	struct _bit_reverse { enum { value = (I & (1u << (BITS - K)) ? (1u << (K - 1u)) : 0) + _bit_reverse<BITS, I, K - 1u>::value }; };

	template<std::size_t N, std::size_t I>
	struct bit_reversed
	{
		enum { value = _bit_reverse<devfix::base::meta::math::log2<N>::value, I, devfix::base::meta::math::log2<N>::value>::value };
	};

}
