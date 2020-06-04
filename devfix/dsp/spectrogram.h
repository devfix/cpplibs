//
// Created by core on 6/3/20.
//

#pragma once

#include <cstddef>
#include <cstring>
#include <complex>
#include <vector>
#include <list>
#include <array>
#include "fft.h"
#include "window.h"

#include <iostream>

namespace devfix::dsp
{

	template<typename FloatT, std::size_t N, FloatT(* win_fun)(std::size_t, std::size_t)>
	struct spectrogram
	{
		using complex_t = std::complex<FloatT>;

		explicit spectrogram(std::size_t window_distance) :
			window_distance_(window_distance)
		{
			if (!window_distance) { throw std::invalid_argument("window distance cannot be zero"); }
		}

		void add(const complex_t* data, std::size_t len)
		{
			std::size_t old_size = temp_data_.size();
			temp_data_.resize(old_size + len);
			std::memcpy(temp_data_.data() + old_size, data, sizeof(complex_t) * len);
			update();
		}

		void add(const std::vector<complex_t>& vec)
		{
			add(vec.data(), vec.size());
		}

		template<std::size_t N_elems>
		void add(const std::array<complex_t, N_elems>& arr)
		{
			add(arr.data(), N_elems);
		}

		std::array<complex_t, N> pop()
		{
			if (data_.empty()) { throw std::out_of_range("spectrogram empty"); }
			std::array<complex_t, N> arr = data_.front();
			data_.pop_front();
			return arr;
		}

		std::size_t size() const
		{
			return data_.size();
		}

	private:
		std::size_t window_distance_;
		std::vector<complex_t> temp_data_;
		std::list<std::array<complex_t, N>> data_;

		void update()
		{
			// calc count of new calculatetable ffts
			std::int32_t n = std::floor((double(temp_data_.size()) - N + 1.) / window_distance_);
			if (n > 0)
			{
				auto ptr = temp_data_.data();
				for (std::size_t k = 0; k++ < n; ptr += window_distance_) { append_fft(ptr); }

				// remove n * window_distance_ elements from beginning of data_
				std::vector<complex_t>(temp_data_.begin() + n * window_distance_, temp_data_.end()).swap(temp_data_);
			}
		}

		void append_fft(complex_t* field)
		{
			auto& window = data_.emplace_back();
			std::memcpy(window.data(), field, sizeof(complex_t) * N);
			std::size_t index = 0;
			auto win_fun_adapt = [&index](const complex_t& complex) { return complex * window::get_window<FloatT, win_fun>(N, index++); };
			std::transform(window.begin(), window.end(), window.begin(), win_fun_adapt);
			fft::transform_inplace<FloatT>(window.data(), N);
		}
	};

}
