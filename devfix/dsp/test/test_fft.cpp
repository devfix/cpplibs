//
// Created by core on 6/1/20.
//


#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include "../fft.h"

using namespace devfix::dsp;

TEST(FFT, Simple)
{
	std::vector<fft::c_t<double>> vec(64);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = std::round(1e6 * (sin(2 * M_PI * double(i) / double(vec.size()) * 4.)
			+ 0.5 * sin(2 * M_PI * double(i) / double(vec.size()) * 2.)
			+ 0.25 * sin(2 * M_PI * double(i) / double(vec.size()) * 1.)
			+ 4)) * 1e-6;
	}

	fft::transform_inplace(vec.data(), vec.size());

	for (auto& z : vec)
	{
		std::cout << std::round(std::abs(z) * 1e3) * 1e-3 << "\n";
	}
}

#endif