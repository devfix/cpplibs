//
// Created by core on 6/1/20.
//

#pragma once

#include <complex>
#include <cmath>
#include <vector>
#include <array>
#include "../base/math.h"
#include "../base/numbers.h"

namespace devfix::dsp
{
	struct fft
	{
		using math  = ::devfix::base::math;

		///////////////////////
		// transform_inplace //
		///////////////////////

		template<typename FloatT>
		static void transform_inplace(std::complex<FloatT>* field, std::size_t len)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			namespace numbers = devfix::base::numbers;
			if (math::exp2(math::floorLog2(len)) != len) { throw std::invalid_argument("len is not a power of 2"); }

			double theta = numbers::pi_v<FloatT> / len;
			std::complex<FloatT> phi = std::complex<FloatT>(cos(theta), -sin(theta)), w;
			for (std::size_t stage = len; stage > 1; stage >>= 1u)
			{
				phi *= phi;
				w = 1;
				for (std::size_t pair = 0; pair < stage / 2; pair++)
				{
					for (std::size_t a = pair; a < len; a += stage)
					{
						std::size_t b = a + stage / 2;
						std::complex<FloatT> t = field[a] - field[b];
						field[a] += field[b];
						field[b] = t * w;
					}
					w *= phi;
				}
			}

			for (std::size_t a = 1; a < len; a++)
			{
				std::size_t b = math::reverse_bits(a, math::popcount(len));
				if (b > a)
				{
					std::complex<FloatT> t = field[a];
					field[a] = field[b];
					field[b] = t;
				}
			}
		}

		template<typename FloatT>
		static void transform_inplace(std::vector<std::complex<FloatT>>& vec)
		{
			transform_inplace<FloatT>(vec.data(), vec.size());
		}

		template<typename FloatT, std::size_t N>
		static void transform_inplace(std::array<std::complex<FloatT>, N>& arr)
		{
			static_assert(N == math::exp2(math::floorLog2(N)), "array size is not a power of 2");  // assert N is power of 2
			transform_inplace<FloatT>(arr.data(), arr.size());
		}

		///////////////////////
		// normalize inplace //
		///////////////////////

		/**
		 * \brief divides each element of the field by the fft size
		 * if the fft size is zero, it gets set to the field length
		 * \tparam FloatT type of floating point numbers
		 * \param field field
		 * \param len input field length
		 * \param n fft size
		 */
		template<typename FloatT>
		static void normalize_inplace(std::complex<FloatT>* field, std::size_t len, std::size_t n = 0)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (n == 0) { n = len; }
			const FloatT inv_n = FloatT(1) / n;
			for (std::size_t i = 0; i < len; i++) { field[i] *= inv_n; }
		}

		/**
		 * \brief divides each element of the vector by the fft size
		 * if the fft size is zero, it gets set to the vector size
		 * \tparam FloatT type of floating point numbers
		 * \param vec vector
		 * \param n fft size
		 */
		template<typename FloatT>
		static void normalize_inplace(std::vector<std::complex<FloatT>>& vec, std::size_t n = 0)
		{
			normalize_inplace<FloatT>(vec.data(), vec.size(), n);
		}

		/**
		 * \brief divides each element of the array by the fft size
		 * if the fft size is zero, it gets set to the array size
		 * \tparam FloatT type of floating point numbers
		 * \tparam N array length
		 * \param arr array
		 * \param n fft size
		 */
		template<typename FloatT, std::size_t N>
		static void normalize_inplace(std::array<std::complex<FloatT>, N>& arr, std::size_t n = 0)
		{
			normalize_inplace<FloatT>(arr.data(), arr.size(), n);
		}

		///////////////////
		// get magnitude //
		///////////////////

		/**
		 * \brief calculates the magnitudes of a normalized fft field
		 * \tparam FloatT type of floating point numbers
		 * \param field input field
		 * \param len input field length
		 * \param mag output field for the magnitudes
		 */
		template<typename FloatT>
		static void get_magnitude(const std::complex<FloatT>* field, std::size_t len, FloatT* mag)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (len == 0) { throw std::invalid_argument("len is not greater zero"); }
			mag[0] = std::abs(field[0]);
			for (std::size_t i = 1; i < len / 2; i++) { mag[i] = std::abs(field[i]) * FloatT(2); }
		}

		/**
		 * \brief calculates the magnitudes of a normalized fft vector
		 * \tparam FloatT type of floating point numbers
		 * \param vec input vector
		 * \return output vector with magnitudes
		 */
		template<typename FloatT>
		static std::vector<FloatT> get_magnitude(const std::vector<std::complex<FloatT>>& vec)
		{
			std::vector<FloatT> mag(vec.size() / 2);
			get_magnitude<FloatT>(vec.data(), vec.size(), mag.data());
			return mag;
		}

		/**
		 * \brief calculates the magnitudes of a normalized fft vector
		 * \tparam FloatT type of floating point numbers
		 * \tparam N input array length
		 * \param arr input array
		 * \return output array with magnitudes
		 */
		template<typename FloatT, std::size_t N>
		static std::array<FloatT, N / 2> get_magnitude(const std::array<std::complex<FloatT>, N>& arr)
		{
			static_assert(N > 0, "len is not greater zero");
			std::array<FloatT, N / 2> mag;
			get_magnitude<FloatT>(arr.data(), arr.size(), mag.data());
			return mag;
		}

		///////////////
		// get phase //
		///////////////

		/**
		 * \brief calculates the phases of a normalized fft field
		 * \tparam FloatT type of floating point numbers
		 * \param field input field
		 * \param len input field length
		 * \param ph output field for the phases
		 */
		template<typename FloatT>
		static void get_phase(const std::complex<FloatT>* field, std::size_t len, FloatT* ph)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			if (len == 0) { throw std::invalid_argument("len is not greater zero"); }
			ph[0] = std::arg(field[0]);
			for (std::size_t i = 1; i < len; i++) { ph[i] = std::arg(field[i]); }
		}

		/**
		 * \brief calculates the phases of a normalized fft vector
		 * \tparam FloatT type of floating point numbers
		 * \param vec input vector
		 * \return output vector with phases
		 */
		template<typename FloatT>
		static std::vector<FloatT> get_phase(const std::vector<std::complex<FloatT>>& vec)
		{
			std::vector<FloatT> ph(vec.size());
			get_phase<FloatT>(vec.data(), vec.size(), ph.data());
			return ph;
		}

		/**
		 * \brief calculates the phases of a normalized fft array
		 * \tparam FloatT type of floating point numbers
		 * \tparam N input array length
		 * \param arr input array
		 * \return output array with phases
		 */
		template<typename FloatT, std::size_t N>
		static std::array<FloatT, N / 2> get_phase(const std::array<std::complex<FloatT>, N>& arr)
		{
			static_assert(N > 0, "len is not greater zero");
			std::array<FloatT, arr.size()> ph;
			get_phase<FloatT>(arr.data(), arr.size(), ph.data());
			return ph;
		}
	};
}
vector