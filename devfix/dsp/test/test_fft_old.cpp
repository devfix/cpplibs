//
// Created by core on 12/19/19.
//

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "../../base/platform.h"
#include "../fft_old.h"
#include "../window.h"
#include "rosettafft.h"

//using namespace devfix::base;

#define ASSERT_ARRAY_EQUAL(arr1, arr2) \
    if (!std::equal(std::begin(arr1), std::end(arr1), std::begin(arr2)))\
    { std::cerr << "ERROR: " << SOURCE_LINE << std::endl; std::exit(1); }

void assert_complex_equals(const std::complex<double>* a, const std::complex<double>* b, std::size_t len)
{
	for (std::size_t i = 0; i < len; i++)
	{
		double r_diff = std::abs(a[i].real() - b[i].real());
		double i_diff = std::abs(a[i].imag() - b[i].imag());
		assert(r_diff < 0.001);
		assert(i_diff < 0.001);
	}
}

void assert_vector_equals(const double* a, const double* b, std::size_t len)
{
	for (std::size_t i = 0; i < len; i++)
	{
		double diff = a[i] - b[i];
		assert(diff < 1e-9);
	}
}

void test_rectangle()
{
	std::vector<std::complex<double>> vect = {
		{ 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }};

	std::vector<std::complex<double>> vect_copy(vect);

	FFT_OLD<4, double>::transform_inplace(vect.data());
	rosetta_fft(vect_copy);

	assert_complex_equals(vect.data(), vect_copy.data(), vect.size());
}

void test_dirac()
{
	std::vector<std::complex<double>> vect = {
		{ 8192 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }};

	std::vector<std::complex<double>> vect_copy(vect);

	FFT_OLD<4, double>::transform_inplace(vect.data());
	rosetta_fft(vect_copy);

	assert_complex_equals(vect.data(), vect_copy.data(), vect.size());
}

void test_periodic()
{
	std::vector<std::complex<double>> vect = {
		{ 0.5 },
		{ 0 },
		{ -0.25 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ -0.25 },
		{ 0 }
	};
	std::vector<std::complex<double>> vect_copy(vect);

	FFT_OLD<3, double>::transform_inplace(vect.data());
	rosetta_fft(vect_copy);

	assert_complex_equals(vect.data(), vect_copy.data(), vect.size());
}

void test_random()
{
	std::vector<std::complex<double>> vect = {
		{ 10 },
		{ 20 },
		{ 30 },
		{ 40 },
		{ 50 },
		{ 60 },
		{ 70 },
		{ 80 }
	};
	std::vector<std::complex<double>> vect_copy(vect);

	FFT_OLD<3, double>::transform_inplace(vect.data());
	rosetta_fft(vect_copy);

	assert_complex_equals(vect.data(), vect_copy.data(), vect.size());
}

void test_random2()
{
	std::vector<std::complex<double>> vect = {
		{ 6 },
		{ 5 },
		{ 6 },
		{ 1 },
		{ 1 },
		{ 6 },
		{ 5 },
		{ 6 }
	};
	std::vector<std::complex<double>> vect_copy(vect);

	FFT_OLD<3, double>::transform_inplace(vect.data());
	rosetta_fft(vect_copy);

	assert_complex_equals(vect.data(), vect_copy.data(), vect.size());
}

void test_hanning()
{
	for (std::size_t k = 0; k < 10; k++)
	{
		std::cout << devfix::dsp::window::hanning(5, k) << std::endl;
	}
}

void test_flattop()
{
	std::vector<double> expected = { -0.000421051000000001, -0.0368407811549235, 0.0107037167161534, 0.78087391493877, 0.78087391493877, 0.0107037167161534, -0.0368407811549235, -0.000421051000000001 };
	std::vector<double> win = { 0, 1, 2, 3, 4, 5, 6, 7 };
	std::transform(win.begin(), win.end(), win.begin(),
				   [&](double& d) { return devfix::dsp::window::FLATTOP_FACTOR * devfix::dsp::window::flattop(win.size(), (int) d); });
	assert_vector_equals(win.data(), expected.data(), expected.size());
}

void test_dsp()
{
	test_flattop();
	test_rectangle();
	test_dirac();
	test_periodic();
	test_random();
	test_random2();
	std::cout << "all tests have passed successfully" << std::endl;
}