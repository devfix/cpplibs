//
// Created by core on 5/25/20.
//

#pragma once
#include <cstdint>
#include <algorithm>
#include <array>
#include <vector>
#include <complex>

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
		static constexpr std::size_t countl_zero(UnsignedT x) noexcept
		{
			static_assert(std::is_unsigned_v<UnsignedT>, "only unsigned types allowed");
			std::size_t cnt = 0;
			for (UnsignedT i = UnsignedT(1u) << (std::numeric_limits<UnsignedT>::digits - 1); i > 0 && !(x & i); i >>= 1, cnt++) {}
			return cnt;
		}

		template<typename UnsignedT>
		static constexpr std::size_t floorLog2(UnsignedT v)
		{
			static_assert(std::is_unsigned_v<UnsignedT>, "only unsigned types allowed");
			if (!v) { throw std::invalid_argument("zero argument"); }
			return sizeof(v) * 8u - 1u - countl_zero(v);
		}

		template<typename UnsignedT>
		static constexpr std::size_t exp2(UnsignedT v)
		{
			static_assert(std::is_unsigned_v<UnsignedT>, "only unsigned types allowed");
			return 1u << v;
		}

		////////////////
		// to complex //
		////////////////

		template<typename FloatT>
		static void to_complex(const FloatT* field, std::size_t len, std::complex<FloatT>* cfield)
		{
			for (std::size_t i = 0; i < len; i++) { cfield[i] = std::complex<FloatT>(field[i]); }
		}

		template<typename FloatT>
		[[nodiscard]] static std::vector<std::complex<FloatT>> to_complex(const std::vector<FloatT>& vec, std::size_t len = 0)
		{
			if (len > vec.size()) { throw std::invalid_argument("bad vector length requested"); }
			if (len == 0) { len = vec.size(); }
			std::vector<std::complex<FloatT>> cvec(len);
			to_complex(vec.data(), len, cvec.data());
			return cvec;
		}

		template<typename FloatT, std::size_t N, std::size_t Len = 0>
		[[nodiscard]] static std::array<std::complex<FloatT>, N> to_complex(const std::array<FloatT, N>& arr)
		{
			static_assert(Len <= N);
			std::array<std::complex<FloatT>, Len == 0 ? N : Len> carr;
			to_complex(arr.data(), carr.size(), carr.data());
			return carr;
		}

		[[nodiscard]] static constexpr std::uint32_t reverse_bits(std::uint32_t val, std::size_t bits)
		{
			val = (((val & 0xaaaaaaaau) >> 1u) | ((val & 0x55555555) << 1u));
			val = (((val & 0xcccccccc) >> 2u) | ((val & 0x33333333) << 2u));
			val = (((val & 0xf0f0f0f0) >> 4u) | ((val & 0x0f0f0f0f) << 4u));
			val = (((val & 0xff00ff00) >> 8u) | ((val & 0x00ff00ff) << 8u));
			return ((val >> 16) | (val << 16)) >> (32 - bits);
		}

		template<typename UnsignedT>
		[[nodiscard]] static constexpr UnsignedT popcount(UnsignedT val)
		{
			static_assert(std::is_unsigned_v<UnsignedT>, "only unsigned types allowed");
			UnsignedT count = 0;
			while (val >>= 1) { count++; }
			return count;
		}

		template<typename T, std::size_t N, typename G, typename ...Args>
		using Table = _math::Table<T, N, G, Args...>;
	};

}
