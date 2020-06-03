//
// Created by core on 6/3/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include "../spectrogram.h"

using namespace devfix::dsp;

auto rect_win = [](std::size_t k) -> double { return 1; };

constexpr std::size_t FFT_LEN = 8;

template<int digits, typename T>
constexpr T round(T val) { return std::round(val * std::pow(10, digits)) * std::pow(10, -digits); }

std::array<double, FFT_LEN> get_magnitudes(const std::array<std::complex<double>, FFT_LEN>& win)
{
	std::array<double, FFT_LEN> mag;
	std::transform(win.begin(), win.end(), mag.begin(), std::abs<double>);
	std::transform(mag.begin(), mag.end(), mag.begin(), round<3, double>);
	return mag;
}

#define ASSERT_ARRAYS_EQ(arr1, arr2, len, th) \
    for(std::size_t i = 0; i < len ; i++) \
    { \
        ASSERT_LT(std::abs(arr1[i] - arr2[i]), th); \
    } \
    (void) len

TEST(Spectrogram, RectWindow)
{
	// generate test data
	std::array<std::complex<double>, 16> test_data;
	test_data.fill(0);
	std::size_t begin_sine_curve = 4;
	std::size_t end_sine_curve = 12;
	for (std::size_t i = begin_sine_curve; i < end_sine_curve; i++)
	{
		double phi = 2 * M_PI * double(i - begin_sine_curve) / double(end_sine_curve - begin_sine_curve);
		test_data[i] = std::sin(phi);
	}

	// create spectrogram and fill it
	spectrogram<double, FFT_LEN, rect_win> spec(1);
	for (auto& c : test_data) { spec.add(&c, 1); }

	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 2.414, 2, 1, 0, 0.414, 0, 1, 2 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 2.414, 2, 1, 0, 0.414, 0, 1, 2 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 1.707, 2.55, 1.225, 0.707, 0.293, 0.707, 1.225, 2.55 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 0.707, 3.536, 0.707, 0.707, 0.707, 0.707, 0.707, 3.536 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 0, 4, 0, 0, 0, 0, 0, 4 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 0, 4, 0, 0, 0, 0, 0, 4 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 0.707, 3.536, 0.707, 0.707, 0.707, 0.707, 0.707, 3.536 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 1.707, 2.55, 1.225, 0.707, 0.293, 0.707, 1.225, 2.55 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes(win);
		std::array<double, FFT_LEN> exptected = { 2.414, 2, 1, 0, 0.414, 0, 1, 2 };
		ASSERT_EQ(mag.size(), exptected.size());
		ASSERT_ARRAYS_EQ(mag, exptected, exptected.size(), 1e-9);
	}

	ASSERT_ANY_THROW(spec.pop());
}

#endif
