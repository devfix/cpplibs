//
// Created by core on 8/12/20.
//

#pragma once

#include <array>
#include <vector>
#include <complex>
#include <stdexcept>
#include <numeric>

#include "winfun.h"

namespace devfix::dsp
{
	/**
	 * \brief buffer for the values of window function for a specific window length
	 * \tparam FloatT type of floating point numbers
	 */
	template<typename FloatT>
	struct window
	{
		static_assert(std::is_floating_point_v<FloatT>);

		/**
		 * \brief create new window and calculate all its values using a window function
		 * \param function window function to calculate the window values with
		 * \param size window size, count of values
		 * \param correct_gain if true, calculates the window's gain and multiplies each window value with it
		 */
		window(winfun_t<FloatT>&& function, std::size_t size, bool correct_gain = true) :
			window_(size)
		{
			// calculate and store all window values
			for (std::size_t i = 0; i < window_.size(); i++) { window_[i] = function(window_.size(), i); }

			// if enabled, calc gain and multiply each window value with it
			if (correct_gain)
			{
				const auto gain = FloatT(window_.size()) / std::accumulate(window_.begin(), window_.end(), FloatT(0));
				for (std::size_t i = 0; i < window_.size(); i++) { window_[i] *= gain; }
			}
		}

		/**
		 * \return count of window values
		 */
		[[nodiscard]] std::size_t size() const { return window_.size(); }

		/**
		 * \brief multiplies each real field element with the window value at the same index,
		 * the length of the field has to be the same as the window size
		 * \param field input field of real numbers
		 */
		void apply(FloatT* field) const
		{
			for (std::size_t i = 0; i < window_.size(); i++) { field[i] *= window_[i]; }
		}

		/**
		 * \brief multiplies each real vector element with the window value at the same index,
		 * the length of the vector has to be the same as the window size
		 * \param vec input vector of real numbers
		 */
		void apply(std::vector<FloatT>& vec) const
		{
			if (vec.size() != window_.size()) { throw std::invalid_argument("mismatch of vector size"); }
			apply(vec.data());
		}

		/**
		 * \brief multiplies each real array element with the window value at the same index,
		 * the length of the array has to be the same as the window size
		 * \tparam N array length
		 * \param arr input array of real numbers
		 */
		template<std::size_t N>
		void apply(std::array<FloatT, N>& arr)
		{
			if (arr.size() != window_.size()) { throw std::invalid_argument("mismatch of array size"); }
			apply(arr.data());
		}

		/**
		 * \brief multiplies each complex field element with the window value at the same index,
		 * the length of the field has to be the same as the window size
		 * \param field input field of complex numbers
		 */
		void apply(std::complex<FloatT>* field) const
		{
			for (std::size_t i = 0; i < window_.size(); i++) { field[i] *= window_[i]; }
		}

		/**
		 * \brief multiplies each complex vector element with the window value at the same index,
		 * the length of the vector has to be the same as the window size
		 * \param vec input vector of complex numbers
		 */
		void apply(std::vector<std::complex<FloatT>>& vec) const
		{
			if (vec.size() != window_.size()) { throw std::invalid_argument("mismatch of vector size"); }
			apply(vec.data());
		}

		/**
		 * \brief multiplies each complex array element with the window value at the same index,
		 * the length of the array has to be the same as the window size
		 * \tparam N array length
		 * \param arr input array of complex numbers
		 */
		template<std::size_t N>
		void apply(std::array<std::complex<FloatT>, N>& arr)
		{
			if (arr.size() != window_.size()) { throw std::invalid_argument("mismatch of array size"); }
			apply(arr.data());
		}

	private:
		std::vector<FloatT> window_; //! pre-calculated window values
	};
}
