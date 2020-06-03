//
// Created by core on 6/3/20.
//

#pragma once

#include <cstddef>
#include <cstring>
#include <complex>
#include <vector>
#include <array>

#include <iostream>

namespace devfix::dsp
{

	template<typename FloatT, std::size_t N>
	struct spectrogram
	{
		using complex_t = std::complex<FloatT>;

		explicit spectrogram(std::size_t window_distance) :
			window_distance_(window_distance)
		{

		}

		void add(const complex_t* data, std::size_t len)
		{
			std::size_t old_size = data_.size();
			data_.resize(old_size + len);
			std::memcpy(data_.data() + sizeof(complex_t) * old_size, data, len);
			update();
		}

		void add(const std::vector<complex_t>& vec)
		{
			add(vec.data(), vec.size());
		}

		template <std::size_t N_elems>
		void add(const std::array<complex_t, N_elems>& arr)
		{
			add(arr.data(), N_elems);
		}

	private:
		std::size_t window_distance_;
		std::vector<complex_t> data_;

		void update()
		{
			// calc count of new calculatetable ffts
			std::size_t n = std::floor(double(data_.size() - N + 1) / window_distance_);
			std::cout << "update: " << n << "\n";
			if (n)
			{
				auto ptr = data_.data();
				for (std::size_t k = 0; k++ < n; ptr += window_distance_) { append_fft(ptr); }

				// remove n * window_distance_ elements from beginning of data_
				std::vector<complex_t>(data_.begin() + n * window_distance_, data_.end()).swap(data_);
			}
		}

		void append_fft(complex_t* field)
		{
			std::cout << "append: " << *field << "\n";
		}
	};

}
