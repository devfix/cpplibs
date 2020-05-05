//
// Created by core on 12/19/19.
//


#include <iostream>
#include <vector>
#include <array>
#include "../../base/platform.h"
#include "../fft.h"

//using namespace devfix::base;

#define ASSERT_ARRAY_EQUAL(arr1, arr2) \
    if (!std::equal(std::begin(arr1), std::end(arr1), std::begin(arr2)))\
    { std::cerr << "ERROR: " << SOURCE_LINE << std::endl; std::exit(1); }

void test_rectangle()
{
	std::complex<double> expected[] = {
		{ 16 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }};
	std::complex<double> vect[] = {
		{ 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }};
	FFT<4, double>::transform_inplace(vect);
	ASSERT_ARRAY_EQUAL(vect, expected);
}

void test_dirac()
{
	std::complex<double> expected[] = {
		{ 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 },
		{ 8192 }, { 8192 }, { 8192 }};
	std::complex<double> vect[] = {
		{ 8192 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }};
	FFT<4, double>::transform_inplace(vect);
	ASSERT_ARRAY_EQUAL(vect, expected);
}

void test_periodic()
{
	std::complex<double> expected[] = {
		{ 0 },
		{ 0.5 },
		{ 1 },
		{ 0.5 },
		{ 0 },
		{ 0.5 },
		{ 1 },
		{ 0.5 },
	};
	std::complex<double> vect[] = {
		{ 0.5 },
		{ 0 },
		{ -0.25 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ -0.25 },
		{ 0 }
	};
	FFT<3, double>::transform_inplace(vect);
	ASSERT_ARRAY_EQUAL(vect, expected);
}

void test_random()
{
	std::complex<double> expected[] = {
		{ 360, 0 },
		{ -40, 97 },
		{ -40, 40 },
		{ -40, 17 },
		{ -40, 0 },
		{ -40, -17 },
		{ -40, -40 },
		{ -40, -97 },
	};
	std::complex<double> vect[] = {
		{ 10 },
		{ 20 },
		{ 30 },
		{ 40 },
		{ 50 },
		{ 60 },
		{ 70 },
		{ 80 }
	};
	FFT<3, double>::transform_inplace(vect);
	std::transform(std::begin(vect), std::end(vect), std::begin(vect), [](auto c)
	{ return decltype(c){ std::round(c.real()), std::round(c.imag()) }; });
	ASSERT_ARRAY_EQUAL(vect, expected);
}


#define T int




void test_dsp()
{

	test_rectangle();
	test_dirac();
	test_periodic();
	test_random();
	std::cout << "all tests have passed successfully" << std::endl;
}