//
// Created by core on 6/3/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include <cstring>
#include "../spectrogram.h"

using namespace devfix::dsp;

TEST(Spectrogram, ComplexTest)
{
	spectrogram<double, 4> spec(1);
	spec.add(std::vector<std::complex<double>>{{ 0 }, { 1 }, { 2 }});
	spec.add(std::vector<std::complex<double>>{{ 3 }, { 4 }, { 5 }});
	spec.add(std::vector<std::complex<double>>{{ 6 }, { 7 }, { 8 }});
	spec.add(std::vector<std::complex<double>>{{ 9 }, { 10 }, { 11 }});
}

#endif
