//
// Created by core on 5/25/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include <cstring>
#include <algorithm>
#include "../window.h"

using namespace devfix::dsp;

TEST(Window, HanningSmall)
{
	std::vector<double> window = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	std::transform(window.begin(), window.end(), window.begin(), [&window](double v)
	{ return std::round(window::hanning(window.size(), (int)v) * 1e6); });

	std::vector<double>
		expected = { 0, 0.079373233584409, 0.292292493499057, 0.571157419136643, 0.827430366972642, 0.979746486807249,
					 0.979746486807249, 0.827430366972642, 0.571157419136643, 0.292292493499057, 0.079373233584409, 0 };
	std::transform(expected.begin(), expected.end(), expected.begin(), [&expected](double v)
	{ return std::round(v * 1e6); });

	ASSERT_EQ(window.size(), expected.size());
	ASSERT_EQ(window, expected);
}

TEST(Window, HanningLarge)
{
	constexpr std::size_t WINDOW_LENGTH = 8192;
	std::vector<double> window = { 0, 128, 512, 1024, 4096, 8191 };
	std::transform(window.begin(), window.end(), window.begin(), [&window](double v)
	{ return std::round(window::hanning(WINDOW_LENGTH, (int)v) * 1e6); });

	std::vector<double>
		expected = { 0, 0.002408224101074, 0.038069407723687, 0.146480511677018, 0.999999963223881, 0 };
	std::transform(expected.begin(), expected.end(), expected.begin(), [&expected](double v)
	{ return std::round(v * 1e6); });

	ASSERT_EQ(window.size(), expected.size());
	EXPECT_EQ(window, expected);
}

TEST(Window, FlatTopSmall)
{
	std::vector<double> window = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	std::transform(window.begin(), window.end(), window.begin(), [&window](double v)
	{ return std::round(window::flattop(window.size(), (int)v) * 1e6); });

	std::vector<double>
		expected = { -0.000421051000000, -0.012389594360634, -0.059860119531311, -0.020035697496734, 0.371978915558499,
					 0.906201246330179, 0.906201246330179, 0.371978915558499, -0.020035697496734, -0.059860119531311,
					 -0.012389594360634, -0.000421051000000 };
	std::transform(expected.begin(), expected.end(), expected.begin(), [&expected](double v)
	{ return std::round(v * 1e6); });

	ASSERT_EQ(window.size(), expected.size());
	EXPECT_EQ(window, expected);
}

TEST(Window, FlatTopLarge)
{
	constexpr std::size_t WINDOW_LENGTH = 8192;
	std::vector<double> window;
	for (std::size_t i = 0; i < 8192; i += 128)
	{ window.push_back(i); }
	std::transform(window.begin(), window.end(), window.begin(), [&window](double v)
	{ return std::round(window::flattop(WINDOW_LENGTH, (int)v) * 1e9); });

	std::vector<double>
		expected =
		{ -4.210661130623662e-04, -6.764098174054656e-04, -0.001479179009699, -0.002955280771981, -0.005291576556518,
		  -0.008702756541970, -0.013386461396966, -0.019468774141331, -0.026943543575953, -0.035610549120719,
		  -0.045019030417189, -0.054424267280366, -0.062765372455649, -0.068671963254625, -0.070505729195380,
		  -0.066440090468441, -0.054577311989413, -0.033097951364930, -4.328822585623927e-04, 0.044556051080384,
		  0.102403809439251, 0.172896929717295, 0.254973361929384, 0.346682790984480, 0.445218164530311,
		  0.547022790048002, 0.647969973023169, 0.743604544684996, 0.829428652419109, 0.901208706703391,
		  0.955277101031420, 0.988801713372102, 0.999998421519452, 0.988266748279888, 0.954235803893566,
		  0.899716156460618, 0.827562207323625, 0.741458088803804, 0.645647110100564, 0.544629602876353,
		  0.442856176141753, 0.344442716436735, 0.252930133875721, 0.171106331467156, 0.100900874496378,
		  0.043355208892993, -0.001336098293849, -0.033723948717462, -0.054958638233414, -0.066617224715409,
		  -0.070522890002585, -0.068573300814229, -0.062591824655402, -0.054211253266338, -0.044795046201633,
		  -0.035396630793478, -0.026753484979897, -0.019309933530183, -0.013260967195880, -0.008608923330132,
		  -0.005225363053770, -0.002911654381586, -0.001453288490115, -6.645021923708675e-04 };
	std::transform(expected.begin(), expected.end(), expected.begin(), [&expected](double v)
	{ return std::round(v * 1e9); });

	// calc abs of difference between value and max it with 1e6
	std::transform(window.begin(), window.end(), expected.begin(), window.begin(), [](double a, double b)
	{ return std::max(std::abs(a - b), 1e6); });

	// set expected vector to 1e6
	std::fill(expected.begin(), expected.end(), 1e6);

	ASSERT_EQ(window.size(), expected.size());
	EXPECT_EQ(window, expected);
}

#endif