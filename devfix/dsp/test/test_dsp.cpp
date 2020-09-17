//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <testutil.h>
#include <fstream>
#include <iostream>
#include "../dsp.h"

using namespace devfix::dsp;
namespace numbers = devfix::base::numbers;


TEST_CASE("devfix/dsp/dsp/calcsignal_multi")
{
	constexpr std::size_t N = 1u << 16u;
	constexpr double fs = 44100;
	constexpr double niq = fs / 2 - 100;
	constexpr double bad = 3.7 * fs / N;

	std::vector<double> vec(N);
	for (std::size_t i = 0; i < vec.size(); i++)
	{
		const double t = double(i) / fs;
		vec[i] = 1.2
			+ 1.0 * std::cos(2 * numbers::pi * bad * t + 1.0)
			+ 1.0 * std::cos(2 * numbers::pi * 220 * t + 0.1)
			+ 0.8 * std::cos(2 * numbers::pi * 440 * t + 0.2)
			+ 0.6 * std::cos(2 * numbers::pi * 880 * t + 0.3)
			+ 0.4 * std::cos(2 * numbers::pi * niq * t + 0.4);
	}

	const auto dftbad = calcsignal(fs, bad, vec);
	const auto dft220 = calcsignal(fs, 220., vec);
	const auto dft440 = calcsignal(fs, 440., vec);
	const auto dft880 = calcsignal(fs, 880., vec);
	const auto dftniq = calcsignal(fs, niq, vec);

	// test magnitudes
	CHECK(std::abs(dftbad) == Approx(1.0).margin(testutil::MARGIN_COARSE));
	CHECK(std::abs(dft220) == Approx(1.0).margin(testutil::MARGIN_COARSE));
	CHECK(std::abs(dft440) == Approx(0.8).margin(testutil::MARGIN_COARSE));
	CHECK(std::abs(dft880) == Approx(0.6).margin(testutil::MARGIN_COARSE));
	CHECK(std::abs(dftniq) == Approx(0.4).margin(testutil::MARGIN_COARSE));

	// test phases
	CHECK(std::arg(dftbad) == Approx(1.0).margin(testutil::MARGIN_COARSE));
	CHECK(std::arg(dft220) == Approx(0.1).margin(testutil::MARGIN_COARSE));
	CHECK(std::arg(dft440) == Approx(0.2).margin(testutil::MARGIN_COARSE));
	CHECK(std::arg(dft880) == Approx(0.3).margin(testutil::MARGIN_COARSE));
	CHECK(std::arg(dftniq) == Approx(0.4).margin(testutil::MARGIN_COARSE));
}

TEST_CASE("devfix/dsp/dsp/calcsignal_all_44100_16")
{
	constexpr std::size_t N = 1u << 16u;
	constexpr double fs = 44100;
	constexpr std::size_t n_test = 128;
	const auto a = double(n_test - 1) / std::log2((fs / 2.) - 3 * fs / N);
	std::vector<double> vec(N);
	for (std::size_t k = 0; k < n_test; k++)
	{
		const double f = std::exp2(double(k) / a) - 1. + fs / N;
		for (std::size_t i = 0; i < vec.size(); i++)
		{
			const double t = double(i) / fs;
			vec[i] = std::cos(2 * numbers::pi * f * t);
		}

		const auto dft = calcsignal(fs, f, vec);
		CHECK(std::abs(dft) == Approx(1.0).margin(testutil::MARGIN_COARSE));
		CHECK(std::arg(dft) == Approx(0).margin(testutil::MARGIN_COARSE));
	}
}

TEST_CASE("devfix/dsp/dsp/calcsignal_all_3200_12")
{
	constexpr std::size_t N = 1u << 12u;
	constexpr double fs = 3200;
	constexpr std::size_t n_test = 64;
	const auto a = double(n_test - 1) / std::log2((fs / 2.) - 3 * fs / N);
	std::vector<double> vec(N);
	for (std::size_t k = 0; k < n_test; k++)
	{
		const double f = std::exp2(double(k) / a) - 1. + fs / N;
		for (std::size_t i = 0; i < vec.size(); i++)
		{
			const double t = double(i) / fs;
			vec[i] = std::cos(2 * numbers::pi * f * t);
		}

		const auto dft = calcsignal(fs, f, vec);
		CHECK(std::abs(dft) == Approx(1.0).margin(testutil::MARGIN_COARSE));
		CHECK(std::arg(dft) == Approx(0).margin(testutil::MARGIN_COARSE));
	}
}

TEST_CASE("devfix/dsp/dsp/calcsignal_all_3200_7")
{
	constexpr std::size_t N = 1u << 7u;
	constexpr double fs = 3200;
	constexpr std::size_t n_test = 64;
	const auto a = double(n_test - 1) / std::log2((fs / 2.) - 3 * fs / N);
	std::vector<double> vec(N);
	for (std::size_t k = 0; k < n_test; k++)
	{
		const double f = std::exp2(double(k) / a) - 1. + fs / N;
		for (std::size_t i = 0; i < vec.size(); i++)
		{
			const double t = double(i) / fs;
			vec[i] = std::cos(2 * numbers::pi * f * t);
		}

		const auto dft = calcsignal(fs, f, vec);
		CHECK(std::abs(dft) == Approx(1.0).margin(testutil::MARGIN_BAD));
		CHECK(std::arg(dft) == Approx(0).margin(testutil::MARGIN_BAD));
	}
}

TEST_CASE("devfix/dsp/dsp/calcrms")
{
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++) { data[i] = 1 + std::sin(2 * numbers::pi * 8 * double(i) / data.size()); }
		CHECK(calcrms(data) == Approx(std::sqrt(1.5)).margin(testutil::MARGIN_FINE));
	}
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++)
		{
			data[i] = 4
				+ 1.00 * std::sin(2 * numbers::pi * 8 * double(i) / data.size())
				+ 0.20 * std::sin(2 * numbers::pi * 64 * double(i) / data.size())
				+ 0.05 * std::sin(2 * numbers::pi * 256 * double(i) / data.size())
				+ 9.00 * std::sin(2 * numbers::pi * 200 * double(i) / data.size());
		}
		CHECK(calcrms(data) == Approx(7.55124162).margin(testutil::MARGIN_FINE));
	}
}

TEST_CASE("devfix/dsp/dsp/calcmean")
{
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++) { data[i] = 1 + std::sin(2 * numbers::pi * 8 * double(i) / data.size()); }
		CHECK(calcmean(data) == Approx(1).margin(testutil::MARGIN_FINE));
	}
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++)
		{
			data[i] = 4
				+ 1.00 * std::sin(2 * numbers::pi * 8 * double(i) / data.size())
				+ 0.20 * std::sin(2 * numbers::pi * 64 * double(i) / data.size())
				+ 0.05 * std::sin(2 * numbers::pi * 256 * double(i) / data.size());
		}
		CHECK(calcmean(data) == Approx(4).margin(testutil::MARGIN_FINE));
	}
}

TEST_CASE("devfix/dsp/dsp/calcacrms")
{
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++) { data[i] = 1 + std::sin(2 * numbers::pi * 8 * double(i) / data.size()); }
		CHECK(calcacrms(data) == Approx(1. / numbers::sqrt2).margin(testutil::MARGIN_FINE));
	}
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++)
		{
			data[i] = 4
				+ 1.00 * std::sin(2 * numbers::pi * 8 * double(i) / data.size())
				+ 0.20 * std::sin(2 * numbers::pi * 64 * double(i) / data.size())
				+ 0.05 * std::sin(2 * numbers::pi * 256 * double(i) / data.size());
		}
		CHECK(calcacrms(data) == Approx(0.721976454).margin(testutil::MARGIN_FINE));
	}
}

TEST_CASE("devfix/dsp/dsp/calcthdn")
{
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++) { data[i] = 1 + std::sin(2 * numbers::pi * 8 * double(i) / data.size()); }
		CHECK(calcthdn(data.size(), 8., data) == Approx(0).margin(testutil::MARGIN_COARSE));
	}
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++)
		{
			data[i] = 4
				+ 1.00 * std::sin(2 * numbers::pi * 8 * double(i) / data.size())
				+ 0.20 * std::sin(2 * numbers::pi * 64 * double(i) / data.size())
				+ 0.05 * std::sin(2 * numbers::pi * 256 * double(i) / data.size());
		}
		CHECK(calcthdn(data.size(), 8., data) == Approx(0.001287879).margin(testutil::MARGIN_COARSE));
		CHECK(calcthdn(data.size(), 64., data) == Approx(0.031289014).margin(testutil::MARGIN_COARSE));
		CHECK(calcthdn(data.size(), 256., data) == Approx(0.031474616).margin(testutil::MARGIN_COARSE));
	}
	{
		std::array<double, 1024> data{};
		for (std::size_t i = 0; i < data.size(); i++)
		{
			data[i] =
				+1.00 * std::sin(2 * numbers::pi * 10 * double(i) / data.size())
					+ 0.20 * std::sin(2 * numbers::pi * 100 * double(i) / data.size())
					+ 0.05 * std::sin(2 * numbers::pi * 200 * double(i) / data.size());
		}
		CHECK(calcthdn(data.size(), 10., data) == Approx(0.0407673861).margin(testutil::MARGIN_COARSE));
		CHECK(calcthdn(data.size(), 100., data) == Approx(0.9616306954).margin(testutil::MARGIN_COARSE));
		CHECK(calcthdn(data.size(), 200., data) == Approx(0.9976019185).margin(testutil::MARGIN_COARSE));
	}
}

#endif
