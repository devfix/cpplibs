//
// Created by core on 8/15/20.
//

#pragma once

#include <cstddef>
#include <type_traits>
#include <cmath>
#include <complex>
#include "../base/numbers.h"

namespace devfix::dsp
{
	/**
	 * \brief calculates the ideal (unrounded) bin index in the spectrum
	 * \tparam FloatT type for floating point numbers, determines precision
	 * \param sample_rate used sample rate
	 * \param fft_len used fft length
	 * \param freq frequency of which the bin index gets calculated
	 * \return
	 */
	template<typename FloatT>
	[[nodiscard]] constexpr FloatT calcfreqbin(std::size_t sample_rate, std::size_t fft_len, FloatT freq)
	{
		static_assert(std::is_floating_point_v<FloatT>);
		return freq * fft_len / sample_rate;
	}

	/**
	 * \brief calculates the closest real (rounded) bin index in the spectrum
	 * \tparam FloatT type for floating point numbers, determines precision
	 * \param sample_rate used sample rate
	 * \param fft_len used fft length
	 * \param freq frequency of which the bin index gets calculated
	 * \return
	 */
	template<typename FloatT>
	[[nodiscard]] constexpr std::size_t calcfreqidx(std::size_t sample_rate, std::size_t fft_len, FloatT freq)
	{
		return std::round(calcfreqbin<FloatT>(sample_rate, fft_len, freq));
	}

	/**
	 * \brief calculates the phase corrector factor for a non coherent frequency
	 * \tparam FloatT type for floating point numbers, determines precision
	 * \param sample_rate
	 * \param fft_len used fft length
	 * \param freq frequency of which the corrector factor gets calculated
	 * \return
	 */
	template<typename FloatT>
	[[nodiscard]] constexpr std::complex<FloatT> calcphasecorrector(std::size_t sample_rate, std::size_t fft_len, FloatT freq)
	{
		const auto offset = calcfreqbin<FloatT>(sample_rate, fft_len, freq) - calcfreqidx<FloatT>(sample_rate, fft_len, freq);
		return std::polar(FloatT(1), -devfix::base::numbers::pi_v<FloatT> * offset);
	}
}
