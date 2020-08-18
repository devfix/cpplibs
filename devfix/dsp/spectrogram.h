//
// Created by core on 8/18/20.
//

#pragma once

#include <vector>
#include <list>
#include <cstring>
#include <optional>
#include "../base/math.h"
#include "types.h"
#include "fft.h"
#include "window.h"

namespace devfix::dsp
{
	template<typename FloatT>
	struct spectrogram
	{
		static_assert(std::is_floating_point_v<FloatT>);

		using math = devfix::base::math;
		using complex_t = std::complex<FloatT>;

		spectrogram(std::size_t fft_len, std::size_t fft_dist, winfun_t<FloatT> winfun = nullptr) :
			fft_len_(fft_len), fft_dist_(fft_dist),
			window_(winfun ? decltype(window_)(window<FloatT>(winfun, fft_len, true)) : std::nullopt)
		{
			if (fft_len_ == 0) { throw std::invalid_argument("fft len invalid"); }
			if (fft_len_ != math::exp2(math::floorLog2(fft_len_))) { throw std::invalid_argument("fft len is has to be power of two"); }
			if (fft_dist_ == 0) { throw std::invalid_argument("fft distance cannot be zero"); }
		}

		/**
		 * \return fft length
		 */
		[[nodiscard]] std::size_t fftlen() { return fft_len_; }

		/**
		 * \return fft distance
		 */
		[[nodiscard]] std::size_t fftdist() { return fft_dist_; }

		/**
		 * \return count of available spectra
		 */
		[[nodiscard]] std::size_t size() { return data_.size(); }

		/**
		 * \return pops the oldest spectrum from the internal list
		 */
		[[nodiscard]] std::vector<complex_t> pop()
		{
			if (data_.empty()) { throw std::out_of_range("spectrogram empty"); }
			std::vector<complex_t> spec = data_.front();
			data_.pop_front();
			return spec;
		}

		void add(const complex_t* field, std::size_t len)
		{
			std::size_t old_size = temp_data_.size();
			temp_data_.resize(old_size + len);
			std::memcpy(temp_data_.data() + old_size, field, sizeof(complex_t) * len);
			update();
		}

		void add(const std::vector<complex_t>& vec) { add(vec.data(), vec.size()); }

		template<std::size_t N>
		void add(const std::array<complex_t, N>& arr) { add(arr.data(), N); }

	private:
		const std::size_t fft_len_;
		const std::size_t fft_dist_;
		const std::optional<window<FloatT>> window_;

		std::vector<complex_t> temp_data_;
		std::list<std::vector<complex_t>> data_;

		void update()
		{
			// calc count of new calculatetable ffts
			std::int32_t n = std::floor((double(temp_data_.size()) - fft_len_ + 1.) / fft_dist_);
			if (n > 0)
			{
				auto ptr = temp_data_.data();
				for (std::size_t k = 0; k < n; k++, ptr += fft_dist_) { append_fft(ptr); }

				// remove n * window_distance_ elements from beginning of data_
				std::vector<complex_t>(temp_data_.begin() + n * fft_dist_, temp_data_.end()).swap(temp_data_);
			}
		}

		void append_fft(complex_t* field)
		{
			auto& win = data_.emplace_back(fft_len_);
			std::memcpy(win.data(), field, sizeof(complex_t) * fft_len_);
			if (window_.has_value()) { window_->apply(win); }
			fft::transform_inplace<FloatT>(win);
		}
	};

}
