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
	FFT<4, double>::fft_transform_inplace(vect);
	ASSERT_ARRAY_EQUAL(vect, expected);
}

void test_dirac()
{
	std::complex<double> expected[] = {
		{ 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 }, { 8192 },
		{ 8192 }, { 8192 }, { 8192 }};
	std::complex<double> vect[] = {
		{ 8192 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }};
	FFT<4, double>::fft_transform_inplace(vect);
	ASSERT_ARRAY_EQUAL(vect, expected);
}

void test_dsp()
{
	test_rectangle();
	test_dirac();
	std::cout << "all tests have passed successfully" << std::endl;
}