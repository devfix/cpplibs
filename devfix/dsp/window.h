//
// Created by core on 8/12/20.
//

#pragma once

#include <array>
#include <vector>
#include <complex>
#include <stdexcept>
#include <numeric>

#include "types.h"

namespace devfix::dsp
{
	template<typename FloatT>
	struct window
	{
		static_assert(std::is_floating_point_v<FloatT>);

		/**
		 * \brief create new window and calculates all values using a window function
		 * \param function window function for this window
		 * \param size window size, count of values
		 * \param correct_gain if true, calculates the window's gain and multiplies each value with it
		 */
		window(winfun_t<FloatT> function, std::size_t size, bool correct_gain = true) :
			window_(size)
		{
			// fill window vector with the values
			for (std::size_t i = 0; i < window_.size(); i++) { window_[i] = function(window_.size(), i); }

			// if enabled, calc gain and multiply window values with it
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
		 * \brief multiplies each field element with the corresponding window value
		 * \param field input field
		 */
		void apply(FloatT* field) const
		{
			for (std::size_t i = 0; i < window_.size(); i++) { field[i] *= window_[i]; }
		}

		/**
		 * \brief multiplies each vector element with the corresponding window value
		 * \param vec input vector
		 */
		void apply(std::vector<FloatT>& vec) const
		{
			if (vec.size() != window_.size()) { throw std::invalid_argument("mismatch of vector size"); }
			apply(vec.data());
		}

		/**
		 * \brief multiplies each array element with the corresponding window value
		 * \tparam N array length
		 * \param arr input array
		 */
		template<std::size_t N>
		void apply(std::array<FloatT, N>& arr)
		{
			if (arr.size() != window_.size()) { throw std::invalid_argument("mismatch of array size"); }
			apply(arr.data());
		}

		/**
		 * \brief multiplies each complex field element with the corresponding window value
		 * \param field input field
		 */
		void apply(std::complex<FloatT>* field) const
		{
			for (std::size_t i = 0; i < window_.size(); i++) { field[i] *= window_[i]; }
		}

		/**
		 * \brief multiplies each complex vector element with the corresponding window value
		 * \param vec input vector
		 */
		void apply(std::vector<std::complex<FloatT>>& vec) const
		{
			if (vec.size() != window_.size()) { throw std::invalid_argument("mismatch of vector size"); }
			apply(vec.data());
		}

		/**
		 * \brief multiplies each complex array element with the corresponding window value
		 * \tparam N array length
		 * \param arr input array
		 */
		template<std::size_t N>
		void apply(std::array<std::complex<FloatT>, N>& arr)
		{
			if (arr.size() != window_.size()) { throw std::invalid_argument("mismatch of array size"); }
			apply(arr.data());
		}

	private:
		std::vector<FloatT> window_; //! window values
	};
}
