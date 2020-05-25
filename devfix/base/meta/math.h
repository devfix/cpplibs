//
// Created by core on 5/25/20.
//

#pragma once
#include <cstdint>

namespace devfix::base::meta::math
{

	template<std::size_t P>
	struct _log2;

	template<>
	struct _log2<1> { enum { value = 0 }; };

	template<std::size_t P>
	struct _log2 { enum { value = 1 + _log2<(P >> 1u)>::value }; };

	template<std::size_t P>
	struct log2
	{
		enum { value = _log2<P>::value };
		static_assert(1u << value == P, "number is not a power of 2");
	};

}
