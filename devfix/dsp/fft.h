//
// Created by core on 05.05.2020.
//

#pragma once

#include <cstdint>
#include <complex>

template<std::size_t BITS, typename T>
struct FFT
{
private:
	template<std::size_t N, std::size_t N_2>
	struct _FFT_WRAPPED
	{
		static void transform_inplace(std::complex<T>* vect)
		{

			size_t k = N, n;
			std::complex<T> phi_t(PHI_T);
			std::complex<T> t;
			while (k)
			{
				n = k;
				k >>= 1u;

				phi_t *= phi_t;

				t.real(1);
				t.imag(0);
				for (size_t l = 0; l < k; l++)
				{
					for (size_t ka = l; ka < N; ka += n)
					{
						const size_t kb = ka + k;
						std::complex<T> diff = vect[ka] - vect[kb];
						vect[ka] += vect[kb];
						vect[kb] = diff * t;
					}
					t *= phi_t;
				}
			}
			order_bit_reversed(vect);

		}

		static void magnitude(std::complex<T>* vect, T* mag)
		{
			for (size_t k = 0; k < N; k++)
			{
				T re = vect[k].real() / N_2;
				T im = vect[k].imag() / N_2;
				mag[k] = re * re + im * im; // skip sqrt of Pythagoras, save squared absolute values directly
			}
		}

	private:
		static constexpr std::complex<T> PHI_T = { cos(M_PI / N), -sin(M_PI / N) };

		static void order_bit_reversed(std::complex<T>* vect)
		{
			for (size_t a = 1; a < N_2; a++)
			{
				size_t b = bit_reverse(a);
				std::complex<T> tmp(vect[a]);
				vect[a] = vect[b];
				vect[b] = tmp;
			}
		}

		static size_t bit_reverse(size_t b)
		{
			b = (b & 0b1010101010101010u) >> 1u | (b & 0b0101010101010101u) << 1u;
			b = (b & 0b1100110011001100u) >> 2u | (b & 0b0011001100110011u) << 2u;
			b = (b & 0b1111000011110000u) >> 4u | (b & 0b0000111100001111u) << 4u;
			b = (b & 0b1111111100000000u) >> 8u | (b & 0b0000000011111111u) << 8u;
			return b >> (16u - BITS);
		}
	};

public:
	static void transform_inplace(std::complex<T>* vect)
	{ _FFT_WRAPPED<1 << BITS, 1 << (BITS - 1)>::transform_inplace(vect); }
};