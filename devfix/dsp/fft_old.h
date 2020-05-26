//
// Created by core on 05.05.2020.
//

#pragma once

#include <cstdint>
#include <complex>

namespace _fft
{

/**
 * \brief computes the bit-reversal index
 * \tparam BITS number width
 * \tparam I index which gets reversed
 * \tparam K current bit (must be BITS at at beginning)
 */
	template<std::size_t BITS, std::size_t I, std::size_t K>
	struct bit_reverse_k;

	template<std::size_t BITS, std::size_t I>
	struct bit_reverse_k<BITS, I, 0>
	{
		enum
		{
			value = 0
		};
	};

	template<std::size_t BITS, std::size_t I, std::size_t K>
	struct bit_reverse_k
	{
		enum
		{
			value = (I & (1 << (BITS - K)) ? (1 << (K - 1)) : 0) + bit_reverse_k<BITS, I, K - 1>::value
		};
	};

/**
 * \brief orders an array bit reversed
 * \tparam T array type
 * \tparam BITS number width
 * \tparam K current index to swap with (must be N/2-1 at beginning)
 */
	template<typename T, std::size_t BITS, std::size_t K>
	struct order_bit_reversed;

	template<typename T, std::size_t BITS>
	struct order_bit_reversed<T, BITS, 0>
	{
		order_bit_reversed(T* vect)
		{};
	};

	template<typename T, std::size_t BITS, std::size_t K>
	struct order_bit_reversed
	{
		order_bit_reversed(T* vect)
		{
			order_bit_reversed<T, BITS, K - 1>{ vect };
			size_t b = bit_reverse_k<BITS, K, BITS>::value;
			T tmp(vect[K]);
			vect[K] = vect[b];
			vect[b] = tmp;
		}
	};

	/**
	 * \brief fft inner loop
	 * \tparam BITS
	 * \tparam T
	 * \tparam K
	 * \tparam L
	 * \tparam R
	 */
	template<std::size_t BITS, typename T, std::size_t K, std::size_t L, std::size_t R>
	struct inner_loop;

	template<std::size_t BITS, typename T, std::size_t K, std::size_t L>
	struct inner_loop<BITS, T, K, L, 0>
	{
		inner_loop(std::complex<T>* vect, std::complex<T>& phi_t, std::complex<T>& t)
		{}
	};

	template<std::size_t BITS, typename T, std::size_t K, std::size_t L, std::size_t R>
	struct inner_loop
	{
		inner_loop(std::complex<T>* vect, std::complex<T>& phi_t, std::complex<T>& t)
		{
			inner_loop<BITS, T, K, L, R - K>{ vect, phi_t, t };
			const size_t kb = R + L - 1 - K + (K >> 1);
			std::complex<T> diff = vect[R + L - 1 - K] - vect[kb];
			vect[R + L - 1 - K] += vect[kb];
			vect[kb] = diff * t;
		}
	};

/**
 * \brief fft midloop
 * \tparam BITS
 * \tparam T
 * \tparam K
 * \tparam L
 */
	template<std::size_t BITS, typename T, std::size_t K, std::size_t L>
	struct mid_loop;

	template<std::size_t BITS, typename T, std::size_t K>
	struct mid_loop<BITS, T, K, 0>
	{
		mid_loop(std::complex<T>* vect, std::complex<T>& phi_t, std::complex<T>& t)
		{}
	};

	template<std::size_t BITS, typename T, std::size_t K, std::size_t L>
	struct mid_loop
	{
		mid_loop(std::complex<T>* vect, std::complex<T>& phi_t, std::complex<T>& t)
		{
			mid_loop<BITS, T, K, L - 1>{ vect, phi_t, t };
			inner_loop<BITS, T, K, L, (1 << BITS) + ((L - 1) % K) - (L - 1)>{ vect, phi_t, t };
			t *= phi_t;
		}
	};

/**
 * \brief outer fft loop
 * \tparam BITS
 * \tparam T
 * \tparam K
 */
	template<std::size_t BITS, typename T, std::size_t K>
	struct outer_loop;

	template<std::size_t BITS, typename T>
	struct outer_loop<BITS, T, 0>
	{
		outer_loop(std::complex<T>* vect, std::complex<T>& phi_t)
		{};
	};

	template<std::size_t BITS, typename T, std::size_t K>
	struct outer_loop
	{
		outer_loop(std::complex<T>* vect, std::complex<T>& phi_t)
		{
			phi_t *= phi_t;
			std::complex<T> t(1);
			mid_loop<BITS, T, K, (K >> 1)>{ vect, phi_t, t };
			outer_loop<BITS, T, (K >> 1)>{ vect, phi_t };
		}
	};

}

template<std::size_t BITS, typename T>
struct FFT
{
	static void transform_inplace(std::complex<T>* vect)
	{
		std::complex<T> phi_t{ cos(M_PI / (1 << BITS)), -sin(M_PI / (1 << BITS)) };
		_fft::outer_loop<BITS, T, (1 << BITS)>{ vect, phi_t };
		_fft::order_bit_reversed<std::complex<T>, BITS, (1 << (BITS - 1)) - 1>{ vect };
	}
};