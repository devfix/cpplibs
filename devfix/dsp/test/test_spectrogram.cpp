//
// Created by core on 6/3/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch/catch.hpp>
#include "../spectrogram.h"

using namespace devfix::dsp;

template<int digits, typename T>
constexpr T round(T val) { return std::round(val * std::pow(10, digits)) * std::pow(10, -digits); }

template<std::size_t FFT_LEN>
std::array<double, FFT_LEN> get_magnitudes(const std::array<std::complex<double>, FFT_LEN>& win)
{
	std::array<double, FFT_LEN> mag;
	std::transform(win.begin(), win.end(), mag.begin(), std::abs<double>);
	return mag;
}

TEST_CASE("Spectrogram - RectangleWindow")
{
	// generate test data
	constexpr std::size_t FFT_LEN = 8;
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
	spectrogram<double, FFT_LEN, window::rectangle> spec(1);
	for (auto& c : test_data) { spec.add(&c, 1); }

	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 2.4142135623730949 / FFT_LEN, 2. / FFT_LEN, 1. / FFT_LEN, 0, 0.4142135623730949 / FFT_LEN, 0, 1. / FFT_LEN, 2. / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 2.4142135623730949 / FFT_LEN, 2. / FFT_LEN, 1. / FFT_LEN, 0, 0.4142135623730947 / FFT_LEN, 0, 1. / FFT_LEN, 2. / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 1.7071067811865479 / FFT_LEN, 2.5495097567963922 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 0.70710678118654768 / FFT_LEN,
			  0.29289321881345265 / FFT_LEN, 0.70710678118654746 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 2.5495097567963922 / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN,
			  0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 0, 4. / FFT_LEN, 0, 0, 0, 0, 0, 4. / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 0, 4. / FFT_LEN, 0, 0, 0, 0, 0, 4. / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN,
			  0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 0.7071067811865479 / FFT_LEN, 3.5355339059327378 / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 1.7071067811865479 / FFT_LEN, 2.5495097567963922 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 0.70710678118654768 / FFT_LEN,
			  0.29289321881345265 / FFT_LEN, 0.70710678118654746 / FFT_LEN, 1.2247448713915889 / FFT_LEN, 2.5495097567963922 / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}
	{
		auto win = spec.pop();
		auto mag = get_magnitudes<FFT_LEN>(win);
		std::array<double, FFT_LEN> expected =
			{ 2.4142135623730949 / FFT_LEN, 2. / FFT_LEN, 1. / FFT_LEN, 0, 0.4142135623730949 / FFT_LEN, 0, 1. / FFT_LEN, 2. / FFT_LEN };
		REQUIRE(mag.size() == expected.size());
		for (std::size_t i = 0; i < mag.size(); i++) { REQUIRE(mag[i] == Approx(expected[i])); }
	}

	REQUIRE_THROWS(spec.pop());
}

TEST_CASE("Spectrogram - FlattopWindow")
{
	// generate test data
	constexpr std::size_t FFT_LEN = 256;
	constexpr std::size_t fs = 256;
	std::array<std::complex<double>, fs> test_data;
	test_data.fill(0);
	for (std::size_t i = 0; i < fs; i++)
	{
		test_data[i] = std::sin(2 * M_PI * fs / 16. * i / fs) + 0.1 * std::sin(2 * M_PI * fs / 4. * i / fs) - 4;
	}

	// create spectrogram and fill it
	int k = 0;
	spectrogram<double, FFT_LEN, window::flattop> spec(fs - FFT_LEN + 1);
	spec.add(test_data);

	// get window, calc abs and divide amplitudes
	auto win = spec.pop();
	auto mag = get_magnitudes<FFT_LEN>(win);
	std::transform(mag.begin(), mag.end(), mag.begin(), [](auto& d) { return d / FFT_LEN; });

	std::array<double, fs> expected = {
		3.999999329229854, 3.867385376577055, 2.587170583638193, 0.791185219869575, 0.068008341276636, 0.000061689156986, 0.000033678236519,
		0.000039534441087, 0.000038424830829, 0.000034736925810, 0.000030423717498, 0.000027986500809, 0.008497355814749, 0.098901444671548,
		0.323393747608726, 0.483425957451071, 0.499998115074673, 0.483425405719979, 0.323394869144165, 0.098899714029679, 0.008499724338979,
		0.000011997324600, 0.000009049400874, 0.000008954950944, 0.000008392152051, 0.000007672930027, 0.000006970394997, 0.000006337937149,
		0.000005783536091, 0.000005300715345, 0.000004879603936, 0.000004510604175, 0.000004185421298, 0.000003897185651, 0.000003640293357,
		0.000003410187731, 0.000003203155208, 0.000003016155387, 0.000002846685947, 0.000002692677528, 0.000002552412781, 0.000002424464501,
		0.000002307648726, 0.000002200987338, 0.000002107100153, 0.000001975906632, 0.000002067328087, 0.000001678133660, 0.000002006246251,
		0.000001556300073, 0.000001826626520, 0.000001611975734, 0.000001623668063, 0.000001597769143, 0.000001584588745, 0.000001578612304,
		0.000001571799485, 0.000001539519575, 0.000001430822330, 0.000001712883321, 0.000849305437404, 0.009890582987818, 0.032338894423458,
		0.048343026595797, 0.049999309463550, 0.048342973299120, 0.032339001199062, 0.009890422355238, 0.000849520387961, 0.000001398750475,
		0.000001044260367, 0.000001087154425, 0.000001050238894, 0.000000985083295, 0.000000916718569, 0.000000853220541, 0.000000819760318,
		0.000000481811685, 0.000001597700535, 0.000000744944923, 0.000002015371817, 0.000000809953385, 0.000001458761873, 0.000000286198575,
		0.000000536041800, 0.000000490128725, 0.000000468541821, 0.000000448270566, 0.000000429167026, 0.000000411094828, 0.000000393939779,
		0.000000377605445, 0.000000362009616, 0.000000347081636, 0.000000332760345, 0.000000318992453, 0.000000305731245, 0.000000292935535,
		0.000000280568831, 0.000000268598641, 0.000000256995920, 0.000000245734614, 0.000000234791295, 0.000000224144876, 0.000000213776440,
		0.000000203669219, 0.000000193808614, 0.000000184172722, 0.000000188121612, 0.000000021342241, 0.000000666124330, 0.000000617395141,
		0.000000927028229, 0.000000635031264, 0.000000631077232, 0.000000051117607, 0.000000119483748, 0.000000098706569, 0.000000091214874,
		0.000000083949274, 0.000000076951461, 0.000000070269623, 0.000000063973184, 0.000000058159993, 0.000000052964362, 0.000000048562842,
		0.000000045169883, 0.000000043011205, 0.000000042268066, 0.000000043011205, 0.000000045169883, 0.000000048562842, 0.000000052964362,
		0.000000058159993, 0.000000063973184, 0.000000070269623, 0.000000076951461, 0.000000083949274, 0.000000091214874, 0.000000098706569,
		0.000000119483748, 0.000000051117607, 0.000000631077232, 0.000000635031264, 0.000000927028229, 0.000000617395141, 0.000000666124330,
		0.000000021342241, 0.000000188121612, 0.000000184172722, 0.000000193808614, 0.000000203669219, 0.000000213776440, 0.000000224144876,
		0.000000234791295, 0.000000245734614, 0.000000256995920, 0.000000268598641, 0.000000280568831, 0.000000292935535, 0.000000305731245,
		0.000000318992453, 0.000000332760345, 0.000000347081636, 0.000000362009616, 0.000000377605445, 0.000000393939779, 0.000000411094828,
		0.000000429167026, 0.000000448270566, 0.000000468541821, 0.000000490128725, 0.000000536041800, 0.000000286198575, 0.000001458761873,
		0.000000809953385, 0.000002015371817, 0.000000744944923, 0.000001597700535, 0.000000481811685, 0.000000819760318, 0.000000853220541,
		0.000000916718569, 0.000000985083295, 0.000001050238894, 0.000001087154425, 0.000001044260367, 0.000001398750475, 0.000849520387961,
		0.009890422355238, 0.032339001199062, 0.048342973299120, 0.049999309463550, 0.048343026595797, 0.032338894423458, 0.009890582987818,
		0.000849305437404, 0.000001712883321, 0.000001430822330, 0.000001539519575, 0.000001571799485, 0.000001578612304, 0.000001584588745,
		0.000001597769143, 0.000001623668063, 0.000001611975734, 0.000001826626520, 0.000001556300073, 0.000002006246251, 0.000001678133660,
		0.000002067328087, 0.000001975906632, 0.000002107100153, 0.000002200987338, 0.000002307648726, 0.000002424464501, 0.000002552412781,
		0.000002692677528, 0.000002846685947, 0.000003016155387, 0.000003203155208, 0.000003410187731, 0.000003640293357, 0.000003897185651,
		0.000004185421298, 0.000004510604175, 0.000004879603936, 0.000005300715345, 0.000005783536091, 0.000006337937149, 0.000006970394997,
		0.000007672930027, 0.000008392152051, 0.000008954950944, 0.000009049400874, 0.000011997324600, 0.008499724338979, 0.098899714029679,
		0.323394869144165, 0.483425405719979, 0.499998115074673, 0.483425957451071, 0.323393747608726, 0.098901444671548, 0.008497355814749,
		0.000027986500809, 0.000030423717498, 0.000034736925810, 0.000038424830829, 0.000039534441087, 0.000033678236519, 0.000061689156986,
		0.068008341276636, 0.791185219869575, 2.587170583638193, 3.867385376577055
	};
	for (auto& v: expected) { v /= FFT_LEN; }  // fft implementation changed, correction now already happens in fft_transform

	REQUIRE(mag.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++)
	{
		REQUIRE(mag[i] == Approx(expected[i])); // ABS_DOUBLE_ERROR is too precise here
	}

	REQUIRE_THROWS(spec.pop());
}

#endif
