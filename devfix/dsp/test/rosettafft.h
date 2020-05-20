//
// Created by core on 20.05.2020.
//

#pragma once

#include <complex>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::vector<Complex> CArray;

void rosetta_fft(CArray& x);
