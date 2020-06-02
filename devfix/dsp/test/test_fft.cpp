//
// Created by core on 6/1/20.
//


#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include <chrono>
#include "../fft.h"

using namespace devfix::dsp;

constexpr std::size_t LEN = 65536;

double perform_test(const std::vector<std::complex<double>>& vec)
{
	auto start = std::chrono::steady_clock::now();
	auto copy(vec);
	fft::transform_inplace<LEN>(copy.data());
	/*for (auto& z : copy)
	{
		std::cout << std::round(std::abs(z) * 1e3) * 1e-3 << "\n";
	}*/
	auto stop = std::chrono::steady_clock::now();
	double time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	return time * 1e-6;
}

using namespace devfix::base::math;

constexpr auto add = [](int a) { return 1 + a; };

TEST(FFT, Simple)
{
	std::vector<std::complex<double>> vec(LEN);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		vec[i] = std::round(1e6 * (sin(2 * M_PI * double(i) / double(vec.size()) * 4.)
			+ 0.5 * sin(2 * M_PI * double(i) / double(vec.size()) * 2.)
			+ 0.25 * sin(2 * M_PI * double(i) / double(vec.size()) * 1.)
			+ 4)) * 1e-6;
	}

	std::size_t n = 1024;
	double min_time = INFINITY;
	for (std::size_t k = 0; k < n; k++) { min_time = std::min(perform_test(vec), min_time); }
	std::cout << "min: " << min_time << "s" << std::endl;
}

#endif