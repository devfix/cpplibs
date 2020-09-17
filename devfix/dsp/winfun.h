//
// Created by core on 8/12/20.
//

#pragma once

#include <cmath>
#include "../base/numbers.h"

/**
 * ~~~Reference~~~
 * Gerhard Heinzel and Albrecht Rüdiger and Roland Schilling:
 * Spectrum and spectral density estimation by the Discrete Fourier transform (DFT), including a comprehensive list of window functions and some new at-top windows
 * http://edoc.mpg.de/395068
 */

namespace devfix::dsp
{
	template<typename FloatT>
	struct winfun_t
	{
		static_assert(std::is_floating_point_v<FloatT>);
		[[nodiscard]] virtual FloatT operator()(std::size_t n, std::size_t k) const = 0;
		[[nodiscard]] virtual FloatT get_nenbw() const = 0;  //! normalized equivalent noise bandwidth
		[[nodiscard]] virtual FloatT get_w3db() const = 0;  //! 3dB bandwidth
		[[nodiscard]] virtual FloatT get_emax() const = 0;  //! maximum amplitude error
	};

	struct winfun
	{
		template<typename FloatT>
		struct rectangle : public winfun_t<FloatT>
		{
			[[nodiscard]] FloatT operator()(std::size_t n, std::size_t k) const override
			{
				return k < n ? FloatT(1) : FloatT(0);
			};

			[[nodiscard]]  FloatT get_nenbw() const override { return 1; };

			[[nodiscard]]  FloatT get_w3db() const override { return 0.8845; };

			[[nodiscard]]  FloatT get_emax() const override { return -36.3380 * 1e-2; };
		};

		template<typename FloatT>
		struct hanning : public winfun_t<FloatT>
		{
			[[nodiscard]] FloatT operator()(std::size_t n, std::size_t k) const override
			{
				namespace numbers = devfix::base::numbers;
				return k < n ? FloatT
					(FloatT(.5)
						 + FloatT(.5) * std::cos(FloatT(2) * numbers::pi_v<FloatT> * (FloatT(k) - FloatT(.5) * (FloatT(n) - 1)) / (n - 1)))
							 : FloatT(0);
			};

			[[nodiscard]]  FloatT get_nenbw() const override { return 1.5; };

			[[nodiscard]]  FloatT get_w3db() const override { return 1.4382; };

			[[nodiscard]]  FloatT get_emax() const override { return -15.1174 * 1e-2; };
		};

		template<typename FloatT>
		struct hamming : public winfun_t<FloatT>
		{
			[[nodiscard]]  FloatT operator()(std::size_t n, std::size_t k) const override
			{
				return get_cosine_win<FloatT>(n, k, { 0.54, -(1 - 0.54) });
			};

			[[nodiscard]]  FloatT get_nenbw() const override { return 1.3628; };

			[[nodiscard]]  FloatT get_w3db() const override { return 1.3008; };

			[[nodiscard]]  FloatT get_emax() const override { return -18.2612 * 1e-2; };
		};

		template<typename FloatT>
		struct flattop_matlab : public winfun_t<FloatT>
		{
			[[nodiscard]]  FloatT operator()(std::size_t n, std::size_t k) const override
			{
				return get_cosine_win<FloatT>(n, k, { 0.21557895, -0.41663158, 0.277263158, -0.083578947, 0.006947368 });
			};

			[[nodiscard]]  FloatT get_nenbw() const override { throw std::runtime_error(__FILE__); };

			[[nodiscard]]  FloatT get_w3db() const override { throw std::runtime_error(__FILE__); };

			[[nodiscard]]  FloatT get_emax() const override { throw std::runtime_error(__FILE__); };
		};

		template<typename FloatT>
		struct flattop_hft248d : public winfun_t<FloatT>
		{
			[[nodiscard]]  FloatT operator()(std::size_t n, std::size_t k) const override
			{
				return get_cosine_win<FloatT>(n, k, {
					1, -1.985844164102, 1.791176438506, -1.282075284005, 0.667777530266, -0.240160796576, 0.056656381764, -0.008134974479,
					0.000624544650, -0.000019808998, 0.000000132974,
				});
			};

			[[nodiscard]]  FloatT get_nenbw() const override { return 5.6512; };

			[[nodiscard]]  FloatT get_w3db() const override { return 5.5567; };

			[[nodiscard]]  FloatT get_emax() const override { return 0.0104 * 1e-2; };
		};

		template<typename FloatT>
		struct flattop_sft5f : public winfun_t<FloatT>
		{
			[[nodiscard]]  FloatT operator()(std::size_t n, std::size_t k) const override
			{
				return get_cosine_win<FloatT>(n, k, {
					0.1881, -0.36923, 0.28702, -0.13077, 0.02488
				});
			};

			[[nodiscard]]  FloatT get_nenbw() const override { return 4.3412; };

			[[nodiscard]]  FloatT get_w3db() const override { return 4.2910; };

			[[nodiscard]]  FloatT get_emax() const override { return 0.0282 * 1e-2; };
		};

		/**
		 * \brief return the window value of a cosine window function
		 * \tparam FloatT type of floating point numbers
		 * \param n length of window
		 * \param k index in window
		 * \param coeffs vector of coefficients for the weighted sum of cosine terms, determines count of terms to add
		 * \return calculated window value
		 */
		template<typename FloatT>
		static FloatT get_cosine_win(std::size_t n, std::size_t k, const std::vector<FloatT>& coeffs)
		{
			static_assert(std::is_floating_point_v<FloatT>);
			namespace numbers = devfix::base::numbers;
			if (k < n)
			{
				FloatT sum = 0;
				for (std::size_t i = 0; i < coeffs.size(); i++)
				{
					sum += coeffs[i] * std::cos(FloatT(2 * numbers::pi_v<FloatT> * i * k) / (n - 1));
				}
				return sum;
			}
			return 0;
		}

	};
}