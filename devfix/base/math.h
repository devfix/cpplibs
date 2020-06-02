//
// Created by core on 5/25/20.
//

#pragma once
#include <cstdint>

namespace devfix::base::math
{
	constexpr std::uint32_t reverse_bits(std::uint32_t val, std::size_t bits)
	{
		val = (((val & 0xaaaaaaaau) >> 1u) | ((val & 0x55555555) << 1u));
		val = (((val & 0xcccccccc) >> 2u) | ((val & 0x33333333) << 2u));
		val = (((val & 0xf0f0f0f0) >> 4u) | ((val & 0x0f0f0f0f) << 4u));
		val = (((val & 0xff00ff00) >> 8u) | ((val & 0x00ff00ff) << 8u));
		return ((val >> 16) | (val << 16)) >> (32 - bits);
	}

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

	template<typename T, std::size_t N, typename G, typename ...Args>
	struct Table
	{
		constexpr Table(G gen, Args ... args) : values()
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				values[i] = gen(i, args...);
			}
		}

		T values[N];
	};

}
