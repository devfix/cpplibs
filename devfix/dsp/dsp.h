//
// Created by core on 8/15/20.
//

#pragma once

#include <cstddef>
#include <type_traits>
#include <cmath>
#include <complex>
#include <vector>
#include "fft.h"
#include "window.h"
#include "winfun.h"
#include "../base/numbers.h"
#include "../base/math.h"

namespace devfix::dsp
{
	using math = ::devfix::base::math;
	namespace numbers = devfix::base::numbers;

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

	////////////////////////
	// calcsignal inplace //
	////////////////////////

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal_inplace(std::size_t sample_rate, FloatT freq, std::complex<FloatT>* field,
		std::size_t len)
	{
		static_assert(std::is_floating_point_v<FloatT>);
		const auto fft_len = math::exp2(math::floorLog2(len));
		window(winfun::flattop_hft248d<FloatT>, fft_len).apply(field);
		fft::transform_inplace(field, fft_len);
		const auto idx = calcfreqidx(sample_rate, fft_len, freq);
		return field[idx] * (FloatT(2) / fft_len) * calcphasecorrector(sample_rate, fft_len, freq);
	}

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal_inplace(std::size_t sample_rate, FloatT freq,
		std::vector<std::complex<FloatT>>& vec)
	{
		return calcsignal_inplace(sample_rate, freq, vec.data(), vec.size());
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] std::complex<FloatT> calcsignal_inplace(std::size_t sample_rate, FloatT freq,
		std::array<std::complex<FloatT>, N>& arr)
	{
		return calcsignal_inplace(sample_rate, freq, arr.data(), arr.size());
	}

	////////////////
	// calcsignal //
	////////////////

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal(std::size_t sample_rate, FloatT freq, const FloatT* field, std::size_t len)
	{
		std::vector<std::complex<FloatT>> cvec(len);
		math::to_complex(field, len, cvec.data());
		return calcsignal_inplace(sample_rate, freq, cvec);
	}

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal(std::size_t sample_rate, FloatT freq, const std::vector<FloatT>& vec)
	{
		auto cvec = math::to_complex(vec);
		return calcsignal_inplace(sample_rate, freq, cvec);
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] std::complex<FloatT> calcsignal(std::size_t sample_rate, FloatT freq, const std::array<FloatT, N>& arr)
	{
		auto carr = math::to_complex(arr);
		return calcsignal_inplace(sample_rate, freq, carr);
	}

	/////////////
	// calcrms //
	/////////////

	template<typename FloatT>
	[[nodiscard]] FloatT calcrms(const FloatT* field, std::size_t len)
	{
		FloatT rms = 0;
		for (std::size_t i = 0; i < len; i++) { rms += field[i] * field[i]; }
		return std::sqrt(rms / len);
	}

	template<typename FloatT>
	[[nodiscard]] FloatT calcrms(const std::vector<FloatT>& vec) { return calcrms<FloatT>(vec.data(), vec.size()); }

	template<typename FloatT, std::size_t N>
	[[nodiscard]] FloatT calcrms(const std::array<FloatT, N>& arr) { return calcrms<FloatT>(arr.data(), arr.size()); }

	//////////////
	// calcmean //
	//////////////

	template<typename FloatT>
	[[nodiscard]] FloatT calcmean(const FloatT* field, std::size_t len)
	{
		FloatT rms = 0;
		for (std::size_t i = 0; i < len; i++) { rms += field[i]; }
		return rms / len;
	}

	template<typename FloatT>
	[[nodiscard]] FloatT calcmean(const std::vector<FloatT>& vec) { return calcmean<FloatT>(vec.data(), vec.size()); }

	template<typename FloatT, std::size_t N>
	[[nodiscard]] FloatT calcmean(const std::array<FloatT, N>& arr) { return calcmean<FloatT>(arr.data(), arr.size()); }

	///////////////
	// calcacrms //
	///////////////

	template<typename FloatT>
	[[nodiscard]] FloatT calcacrms(const FloatT* field, std::size_t len)
	{
		auto rms = calcrms(field, len);
		auto mean = calcmean(field, len);
		return std::sqrt(rms * rms - mean * mean);
	}

	template<typename FloatT>
	[[nodiscard]] FloatT calcacrms(const std::vector<FloatT>& vec) { return calcacrms<FloatT>(vec.data(), vec.size()); }

	template<typename FloatT, std::size_t N>
	[[nodiscard]] FloatT calcacrms(const std::array<FloatT, N>& arr) { return calcacrms<FloatT>(arr.data(), arr.size()); }

	//////////////
	// calcthdn //
	//////////////

	template<typename FloatT>
	[[nodiscard]] FloatT calcthdn(std::size_t sample_rate, FloatT freq, const FloatT* field, std::size_t len)
	{
		const auto fft_len = math::exp2(math::floorLog2(len));
		auto dc = calcmean(field, fft_len);
		auto ac = std::abs(calcsignal(sample_rate, freq, field, fft_len)) / numbers::sqrt2;
		auto signal = dc * dc + ac * ac;
		auto rms = calcrms(field, fft_len);
		return (rms * rms - signal) / signal;
	}

	template<typename FloatT>
	[[nodiscard]] FloatT calcthdn(std::size_t sample_rate, FloatT freq, const std::vector<FloatT>& vec)
	{
		return calcthdn(sample_rate, freq, vec.data(), vec.size());
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] FloatT calcthdn(std::size_t sample_rate, FloatT freq, const std::array<FloatT, N>& arr)
	{
		return calcthdn(sample_rate, freq, arr.data(), arr.size());
	}
}
