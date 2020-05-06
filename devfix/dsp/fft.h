//
// Created by core on 05.05.2020.
//

#pragma once

#include <cstdint>
#include <complex>

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
	template<std::size_t, std::size_t, std::size_t>
	friend
	struct bit_reverse_k;
private:
	enum
	{
		value = 0
	};
};

template<std::size_t BITS, std::size_t I, std::size_t K>
struct bit_reverse_k
{
	template<std::size_t, std::size_t, std::size_t>
	friend
	struct bit_reverse_k;
	template<typename, std::size_t, std::size_t>
	friend
	struct order_bit_reversed;
private:
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
	template<typename, std::size_t, std::size_t> friend
	class order_bit_reversed;
private:
	order_bit_reversed(T* vect)
	{};
};

template<typename T, std::size_t BITS, std::size_t K>
struct order_bit_reversed
{
	template<std::size_t, typename> friend
	class FFT;
	template<typename, std::size_t, std::size_t> friend
	class order_bit_reversed;
private:
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
 * \brief fft midloop
 * \tparam BITS
 * \tparam T
 * \tparam K
 * \tparam L
 */
template<std::size_t BITS, typename T, std::size_t K, std::size_t L>
struct fft_mid_loop;

template<std::size_t BITS, typename T, std::size_t K>
struct fft_mid_loop<BITS, T, K, 0>
{
	fft_mid_loop(std::complex<T>* vect, std::complex<T>& phi_t, std::complex<T>& t)
	{}
};

template<std::size_t BITS, typename T, std::size_t K, std::size_t L>
struct fft_mid_loop
{
	fft_mid_loop(std::complex<T>* vect, std::complex<T>& phi_t, std::complex<T>& t)
	{
		fft_mid_loop<BITS, T, K, L-1>{ vect, phi_t, t};
		for (size_t ka = (L-1); ka < (1 << BITS); ka += K)
		{
			const size_t kb = ka + (K >> 1);
			std::complex<T> diff = vect[ka] - vect[kb];
			vect[ka] += vect[kb];
			vect[kb] = diff * t;
		}
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
struct fft_outer_loop;

template<std::size_t BITS, typename T>
struct fft_outer_loop<BITS, T, 0>
{
	fft_outer_loop(std::complex<T>* vect, std::complex<T>& phi_t)
	{};
};

template<std::size_t BITS, typename T, std::size_t K>
struct fft_outer_loop
{
	fft_outer_loop(std::complex<T>* vect, std::complex<T>& phi_t)
	{
		phi_t *= phi_t;
		std::complex<T> t(1);
		fft_mid_loop<BITS, T, K, (K >> 1)>{ vect, phi_t, t};
		fft_outer_loop<BITS, T, (K >> 1)>{ vect, phi_t };
	}
};


template<std::size_t BITS, typename T>
struct FFT
{
private:
	template<std::size_t N, std::size_t N_2>
	struct _FFT_WRAPPED
	{
		static void transform_inplace(std::complex<T>* vect)
		{

			std::complex<T> phi_t(PHI_T);
			fft_outer_loop<BITS, T, N>{ vect, phi_t };
			order_bit_reversed<std::complex<T>, BITS, N_2 - 1>{ vect };
		}

	private:
		static constexpr std::complex<T> PHI_T = { cos(M_PI / N), -sin(M_PI / N) };
	};

public:
	static void transform_inplace(std::complex<T>* vect)
	{ _FFT_WRAPPED<1 << BITS, 1 << (BITS - 1)>::transform_inplace(vect); }
};