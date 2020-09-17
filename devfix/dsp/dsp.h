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

	/**
	 * \brief calculates an dft coefficient of the discrete Fourier-transformation
	 * \tparam FloatT floating point type
	 * \param field
	 * \param len
	 * \param k
	 * \return
	 */
	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::complex<FloatT>* field, std::size_t len, std::size_t k)
	{
		static_assert(std::is_floating_point_v<FloatT>);
		const auto wn = std::polar(FloatT(1), FloatT(numbers::pi * 2. * k / len));
		std::complex<FloatT> s = field[0];
		for (std::int32_t i = 1; i < len; i++)
		{
			s *= wn;
			s += field[i];
		}
		return s * wn;
	}

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::vector<std::complex<FloatT>>& vec, std::size_t k)
	{
		return goertzel(vec.data(), vec.size(), k);
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] std::complex<FloatT> goertzel(const std::array<std::complex<FloatT>, N>& arr, std::size_t k)
	{
		return goertzel(arr.data(), arr.size(), k);
	}

	////////////////////////
	// calcsignal inplace //
	////////////////////////

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal_inplace(double sample_rate, FloatT freq, std::complex<FloatT>* field,
		std::size_t len)
	{
		static_assert(std::is_floating_point_v<FloatT>);
		auto idx = std::round(calcfreqbin<FloatT>(sample_rate, len, freq));

		auto winfun = winfun::flattop_sft5f<FloatT>();
		const auto enbw = winfun.get_nenbw() * sample_rate / len;

		FloatT ampl_corr = 2.0;
		const std::size_t threshold = std::round(enbw * 6 + 2);
		if(idx == 0)
		{
			ampl_corr = 1.0;
			window(std::move(winfun), len).apply(field);
		}
		else if (idx > threshold)
		{
			window(std::move(winfun), len).apply(field);
		}
		else
		{
			idx = std::floor(calcfreqbin<FloatT>(sample_rate, len, freq));
			len = std::min(std::size_t(std::round(sample_rate * idx / freq)), len);
		}
		const auto dft = goertzel(field, len, idx);
		return dft * (ampl_corr / len) * calcphasecorrector(sample_rate, len, freq);
	}

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal_inplace(double sample_rate, FloatT freq,
		std::vector<std::complex<FloatT>>& vec)
	{
		return calcsignal_inplace(sample_rate, freq, vec.data(), vec.size());
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] std::complex<FloatT> calcsignal_inplace(double sample_rate, FloatT freq,
		std::array<std::complex<FloatT>, N>& arr)
	{
		return calcsignal_inplace(sample_rate, freq, arr.data(), arr.size());
	}

	////////////////
	// calcsignal //
	////////////////

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal(double sample_rate, FloatT freq, const FloatT* field, std::size_t len)
	{
		std::vector<std::complex<FloatT>> cvec(len);
		math::to_complex(field, len, cvec.data());
		return calcsignal_inplace(sample_rate, freq, cvec);
	}

	template<typename FloatT>
	[[nodiscard]] std::complex<FloatT> calcsignal(double sample_rate, FloatT freq, const std::vector<FloatT>& vec)
	{
		auto cvec = math::to_complex(vec);
		return calcsignal_inplace(sample_rate, freq, cvec);
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] std::complex<FloatT> calcsignal(double sample_rate, FloatT freq, const std::array<FloatT, N>& arr)
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
	[[nodiscard]] FloatT calcthdn(FloatT sample_rate, FloatT freq, const FloatT* field, std::size_t len)
	{
		const auto idx = std::floor(freq / sample_rate * len);
		const auto fft_len = std::min(std::size_t(std::round(sample_rate * idx / freq)), len);
		const auto rms = calcrms(field, fft_len);
		const auto dc = calcmean(field, fft_len);
		const auto ac = std::abs(calcsignal(sample_rate, freq, field, fft_len)) / numbers::sqrt2;
		const auto p_total = rms * rms;
		const auto p_signal = dc * dc + ac * ac;
		const auto thdn = (p_total - p_signal) / p_total;
		return std::clamp(thdn, FloatT(0), FloatT(1));
	}

	template<typename FloatT>
	[[nodiscard]] FloatT calcthdn(FloatT sample_rate, FloatT freq, const std::vector<FloatT>& vec)
	{
		return calcthdn(sample_rate, freq, vec.data(), vec.size());
	}

	template<typename FloatT, std::size_t N>
	[[nodiscard]] FloatT calcthdn(FloatT sample_rate, FloatT freq, const std::array<FloatT, N>& arr)
	{
		return calcthdn(sample_rate, freq, arr.data(), arr.size());
	}
}
