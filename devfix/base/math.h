//
// Created by core on 5/25/20.
//

#pragma once
#include <cstdint>
#include <algorithm>
#include <array>
#include <vector>
#include <ccomplex>

namespace devfix::base
{

	namespace _math
	{
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

	struct math
	{
		template<class UnsignedT>
		static constexpr int countl_zero(UnsignedT x) noexcept
		{
			static_assert(std::is_unsigned_v<UnsignedT>, "only unsigned types allowed");
			int cnt = 0;
			for (UnsignedT i = UnsignedT(1u) << (std::numeric_limits<UnsignedT>::digits - 1); i > 0 && !(x & i); i >>= 1, cnt++) {}
			return cnt;
		}

		template<typename UnsignedT>
		static std::size_t floorLog2(UnsignedT v)
		{
			static_assert(std::is_unsigned_v<UnsignedT>, "only unsigned types allowed");
			if (!v) { throw std::invalid_argument("zero argument"); }
			return sizeof(v) * 8 - 1 - countl_zero(v);
		}

		template<typename FloatT, std::size_t N>
		[[nodiscard]] static std::array<std::complex<FloatT>, N> to_complex(const std::array<FloatT, N>& arr)
		{
			std::array<std::complex<FloatT>, N> carr;
			std::transform(arr.begin(), arr.end(), carr.begin(), [](const FloatT& v) { return std::complex<FloatT>(v); });
			return carr;
		}

		template<typename FloatT>
		[[nodiscard]] static std::vector<std::complex<FloatT>> to_complex(const std::vector<FloatT>& vec)
		{
			std::vector<std::complex<FloatT>> cvec(vec.size());
			std::transform(vec.begin(), vec.end(), cvec.begin(), [](const FloatT& v) { return std::complex<FloatT>(v); });
			return cvec;
		}

		[[nodiscard]] static constexpr std::uint32_t reverse_bits(std::uint32_t val, std::size_t bits)
		{
			val = (((val & 0xaaaaaaaau) >> 1u) | ((val & 0x55555555) << 1u));
			val = (((val & 0xcccccccc) >> 2u) | ((val & 0x33333333) << 2u));
			val = (((val & 0xf0f0f0f0) >> 4u) | ((val & 0x0f0f0f0f) << 4u));
			val = (((val & 0xff00ff00) >> 8u) | ((val & 0x00ff00ff) << 8u));
			return ((val >> 16) | (val << 16)) >> (32 - bits);
		}

		template<typename T, class = typename std::enable_if<std::is_unsigned<T>::value>::type>
		[[nodiscard]] static constexpr T popcount(T val)
		{
			T count = 0;
			while (val >>= 1) { count++; }
			return count;
		}

		template<typename T, std::size_t N, typename G, typename ...Args>
		using Table = _math::Table<T, N, G, Args...>;
	};

}
