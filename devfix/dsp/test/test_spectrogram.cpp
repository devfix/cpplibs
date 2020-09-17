//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <testutil.h>
#include "../spectrogram.h"

using namespace devfix::dsp;
namespace numbers = devfix::base::numbers;

std::vector<double> get_magnitudes(const std::vector<std::complex<double>>& win)
{
	std::vector<double> mag(win.size());
	std::transform(win.begin(), win.end(), mag.begin(), std::abs<double>);
	return mag;
}

TEST_CASE("devfix/dsp/spectrogram/rectangle")
{
	// generate test data
	constexpr std::size_t FFT_LEN = 8;
	std::array<std::complex<double>, 16> test_data;
	test_data.fill(0);
	std::size_t begin_sine_curve = 4;
	std::size_t end_sine_curve = 12;
	for (std::size_t i = begin_sine_curve; i < end_sine_curve; i++)
	{
		double phi = 2 * numbers::pi * double(i - begin_sine_curve) / double(end_sine_curve - begin_sine_curve);
		test_data[i] = std::sin(phi);
	}

	// create spectrogram and fill it
	spectrogram<double> spec(FFT_LEN, 1, winfun::rectangle<double>());
	for (auto& c : test_data) { spec.add(&c, 1); }

	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 2.4142135623730949 / FFT_LEN, 2. / FFT_LEN, 1. / FFT_LEN, 0, 0.4142135623730949 / FFT_LEN, 0, 1. / FFT_LEN, 2. / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 2.4142135623730949 / FFT_LEN, 2. / FFT_LEN, 1. / FFT_LEN, 0, 0.4142135623730947 / FFT_LEN, 0, 1. / FFT_LEN, 2. / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 1.7071067811865479 / FFT_LEN, 2.5495097567963922 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 0.70710678118654768 / FFT_LEN,
			  0.29289321881345265 / FFT_LEN, 0.70710678118654746 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 2.5495097567963922 / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN,
			  0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 0, 4. / FFT_LEN, 0, 0, 0, 0, 0, 4. / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 0, 4. / FFT_LEN, 0, 0, 0, 0, 0, 4. / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN,
			  0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 1.7071067811865479 / FFT_LEN, 2.5495097567963922 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 0.70710678118654768 / FFT_LEN,
			  0.29289321881345265 / FFT_LEN, 0.70710678118654746 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 2.5495097567963922 / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}
	{
		auto win = spec.pop();
		fft::normalize_inplace(win);
		auto mag = get_magnitudes(win);
		std::vector<double> expected =
			{ 2.4142135623730949 / FFT_LEN, 2. / FFT_LEN, 1. / FFT_LEN, 0, 0.4142135623730949 / FFT_LEN, 0, 1. / FFT_LEN, 2. / FFT_LEN };
		testutil::check_equals(mag, expected, testutil::MARGIN_FINE);
	}

	REQUIRE_THROWS(spec.pop());
}

#endif
