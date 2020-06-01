//
// Created by core on 6/1/20.
//

#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "addr.h"

namespace devfix::dsp::fft
{
	template<typename T>
	using c_t = std::complex<T>;

	namespace _fft
	{
		template<typename T>
		void order_bit_reversed(c_t<T>* vec, std::size_t len)
		{
			std::size_t bits = std::log2(len);
			for (std::size_t a = 0; a < len; a++)
			{
				std::size_t b = a;
				// Reverse bits
				b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
				b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
				b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
				b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
				b = ((b >> 16) | (b << 16)) >> (32 - bits);
				if (b > a)
				{
					c_t<T> t = vec[a];
					vec[a] = vec[b];
					vec[b] = t;
				}
			}
		}
	}

	template <std::size_t N, std::size_t>
	struct Table;

	template <std::size_t N>
	struct Table<N, N>
	{};

	template <std::size_t N, std::size_t I = 0>
	struct Table
	{
		std::size_t value = bit_reversed<N, I>::value;
		Table<N, I+1> next;
	} __attribute__ ((packed));

	template<typename T>
	void transform_inplace(c_t<T>* vec, std::size_t len)
	{
		_fft::order_bit_reversed(vec, len);

		Table<8> t;
		std::size_t* adr = reinterpret_cast<std::size_t*>(&t);

		for (std::size_t stage = 1; stage <= len; stage <<= 1)
		{
			for (std::size_t n = 0; n < stage / 2; n++)
			{
				for (std::size_t a = n; a < len; a += stage)
				{
					std::size_t b = a + stage / 2;
					c_t<T> wb = std::polar(1., -2 * M_PI * n / double(stage)) * vec[b];
					vec[b] = vec[a] - wb;
					vec[a] += wb;
				}
			}
		}
	}
}
