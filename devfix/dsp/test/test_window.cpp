//
// Created by core on 5/25/20.
//

#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch/catch.hpp>
#include <cstring>
#include "../window.h"

using namespace devfix::dsp;

template<typename FloatT, FloatT(* win_fun)(std::size_t, std::size_t)>
std::vector<FloatT> create_window(std::size_t n)
{
	std::vector<FloatT> win(n);
	for (std::size_t i = 0; i < n; i++) { win[i] = win_fun(n, i); }
	return win;
}

TEST_CASE("Window - HanningGain")
{
	{
		auto gain = window::calc_amplitude_gain<double, window::hanning<double>>(8);
		double gain_expected = 2.285714285714286;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::hanning<double>>(128);
		double gain_expected = 2.015748031496062;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::hanning<double>>(512);
		double gain_expected = 2.003913894324854;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::hanning<double>>(16384);
		double gain_expected = 2.000122077763540;
		CHECK(gain == Approx(gain_expected));
	}
}

TEST_CASE("Window - HanningFloatSmall")
{
	using FloatT = float;
	auto win = create_window<FloatT, window::hanning<FloatT>>(8);

	std::vector<FloatT> expected = {
		0, 0.188255099070633, 0.611260466978157, 0.950484433951210, 0.950484433951210, 0.611260466978157, 0.188255099070633, 0
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - HanningDoubleSmall")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::hanning<FloatT>>(8);

	std::vector<FloatT> expected = {
		0, 0.188255099070633, 0.611260466978157, 0.950484433951210, 0.950484433951210, 0.611260466978157, 0.188255099070633, 0
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - HanningDoubleLarge")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::hanning<FloatT>>(256);

	std::vector<FloatT> expected = {
		0, 0.000151774011064, 0.000607003902855, 0.001365413307106, 0.002426541796468, 0.003789745164032, 0.005454195814427,
		0.007418883266247, 0.009682614765511, 0.012244016009782, 0.015101531982495, 0.018253427897008, 0.021697790249780, 0.025432527982049,
		0.029455373749314, 0.033763885297822, 0.038355446947255, 0.043227271178700, 0.048376400326936, 0.053799708376026, 0.059493902857108,
		0.065455526847236, 0.071680961068069, 0.078166426083117, 0.084907984592225, 0.091901543821889, 0.099142858009967, 0.106627530983258,
		0.114351018826410, 0.122308632640503, 0.130495541389670, 0.138906774833995, 0.147537226546926, 0.156381657015369, 0.165434696820571,
		0.174690849897879, 0.184144496873374, 0.193789898475375, 0.203621199018723, 0.213632429959747, 0.223817513519747, 0.234170266374782,
		0.244684403409547, 0.255353541533038, 0.266171203553707, 0.277130822111731, 0.288225743666038, 0.299449232533641, 0.310794474978845,
		0.322254583349841, 0.333822600260170, 0.345491502812526, 0.357254206862329, 0.369103571318480, 0.381032402478691, 0.393033458396751,
		0.405099453279087, 0.417223061907935, 0.429396924088454, 0.441613649117072, 0.453865820268349, 0.466145999297646, 0.478446730956852,
		0.490760547520435, 0.503079973319069, 0.515397529278085, 0.527705737457985, 0.539997125594271, 0.552264231633827, 0.564499608265101,
		0.576695827439343, 0.588845484880134, 0.600941204578505, 0.612975643270874, 0.624941494897115, 0.636831495036041, 0.648638425315601,
		0.660355117795127, 0.671974459316964, 0.683489395824836, 0.694892936646340, 0.706178158736952, 0.717338210882982, 0.728366317860920,
		0.739255784550643, 0.750000000000000, 0.760592441438293, 0.771026678236225, 0.781296375809912, 0.791395299466580, 0.801317318189628,
		0.811056408360737, 0.820606657416789, 0.829962267439361, 0.839117558674617, 0.848066972981463, 0.856805077205876, 0.865326566479347,
		0.873626267439445, 0.881699141370551, 0.889540287262835, 0.897144944787643, 0.904508497187474, 0.911626474078794, 0.918494554165989,
		0.925108567864807, 0.931464499833695, 0.937558491411496, 0.943386842960031, 0.948946016110129, 0.954232635909762, 0.959243492872962,
		0.963975544928287, 0.968425919265655, 0.972591914080410, 0.976471000213578, 0.980060822687314, 0.983359202134594, 0.986364136122302,
		0.989073800366903, 0.991486549841951, 0.993600919776785, 0.995415626545780, 0.996929568447637, 0.998141826374215, 0.999051664368522,
		0.999658530071512, 0.999962055057415, 0.999962055057415, 0.999658530071512, 0.999051664368522, 0.998141826374215, 0.996929568447637,
		0.995415626545780, 0.993600919776785, 0.991486549841951, 0.989073800366903, 0.986364136122302, 0.983359202134594, 0.980060822687314,
		0.976471000213578, 0.972591914080410, 0.968425919265655, 0.963975544928287, 0.959243492872962, 0.954232635909762, 0.948946016110129,
		0.943386842960031, 0.937558491411496, 0.931464499833695, 0.925108567864807, 0.918494554165989, 0.911626474078794, 0.904508497187474,
		0.897144944787643, 0.889540287262835, 0.881699141370551, 0.873626267439445, 0.865326566479347, 0.856805077205876, 0.848066972981463,
		0.839117558674617, 0.829962267439361, 0.820606657416789, 0.811056408360737, 0.801317318189628, 0.791395299466580, 0.781296375809912,
		0.771026678236225, 0.760592441438293, 0.750000000000000, 0.739255784550643, 0.728366317860920, 0.717338210882982, 0.706178158736952,
		0.694892936646340, 0.683489395824836, 0.671974459316964, 0.660355117795127, 0.648638425315601, 0.636831495036041, 0.624941494897115,
		0.612975643270874, 0.600941204578505, 0.588845484880134, 0.576695827439343, 0.564499608265101, 0.552264231633827, 0.539997125594271,
		0.527705737457985, 0.515397529278085, 0.503079973319069, 0.490760547520435, 0.478446730956852, 0.466145999297646, 0.453865820268349,
		0.441613649117072, 0.429396924088454, 0.417223061907935, 0.405099453279087, 0.393033458396751, 0.381032402478691, 0.369103571318480,
		0.357254206862329, 0.345491502812526, 0.333822600260170, 0.322254583349841, 0.310794474978845, 0.299449232533641, 0.288225743666038,
		0.277130822111731, 0.266171203553707, 0.255353541533038, 0.244684403409547, 0.234170266374782, 0.223817513519747, 0.213632429959747,
		0.203621199018723, 0.193789898475375, 0.184144496873374, 0.174690849897879, 0.165434696820571, 0.156381657015369, 0.147537226546926,
		0.138906774833995, 0.130495541389670, 0.122308632640503, 0.114351018826410, 0.106627530983258, 0.099142858009967, 0.091901543821889,
		0.084907984592225, 0.078166426083117, 0.071680961068069, 0.065455526847236, 0.059493902857108, 0.053799708376026, 0.048376400326936,
		0.043227271178700, 0.038355446947255, 0.033763885297822, 0.029455373749314, 0.025432527982049, 0.021697790249780, 0.018253427897008,
		0.015101531982495, 0.012244016009782, 0.009682614765511, 0.007418883266247, 0.005454195814427, 0.003789745164032, 0.002426541796468,
		0.001365413307106, 0.000607003902855, 0.000151774011064, 0
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - HammingGain")
{
	{
		auto gain = window::calc_amplitude_gain<double, window::hamming<double>>(8);
		double gain_expected = 2.072538860103627;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::hamming<double>>(128);
		double gain_expected = 1.864258665889892;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::hamming<double>>(512);
		double gain_expected = 1.854938047967539;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::hamming<double>>(16384);
		double gain_expected = 1.851948140026453;
		CHECK(gain == Approx(gain_expected));
	}
}

TEST_CASE("Window - HammingSmall")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::hamming<FloatT>>(8);

	std::vector<FloatT> expected = {
		0.080000000000000, 0.253194691144983, 0.642359629619905, 0.954445679235113, 0.954445679235113, 0.642359629619905, 0.253194691144983,
		0.080000000000000
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - HammingLarge")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::hamming<FloatT>>(256);

	std::vector<FloatT> expected = {
		0.080000000000000, 0.080139632090179, 0.080558443590627, 0.081256180242538, 0.082232418452750, 0.083486565550910, 0.085017860149273,
		0.086825372604947, 0.088908005584270, 0.091264494728999, 0.093893409423896, 0.096793153665248, 0.099961967029797, 0.103397925743485,
		0.107098943849369, 0.111062774473996, 0.115287011191475, 0.119769089484404, 0.124506288300781, 0.129495731705944, 0.134734390628539,
		0.140219084699457, 0.145946484182623, 0.151913111996467, 0.158115345824847, 0.164549420316138, 0.171211429369169, 0.178097328504598,
		0.185202937320297, 0.192523942029263, 0.200055898078497, 0.207794232847276, 0.215734248423172, 0.223871124454139, 0.232199921074925,
		0.240715581906049, 0.249412937123505, 0.258286706597345, 0.267331503097225, 0.276541835562968, 0.285912112438167, 0.295436645064799,
		0.305109651136783, 0.314925258210395, 0.324877507269410, 0.334960356342792, 0.345167684172755, 0.355493293930949, 0.365930916980537,
		0.376474216681854, 0.387116792239357, 0.397852182587524, 0.408673870313343, 0.419575285613002, 0.430549810280395, 0.441590781725011,
		0.452691497016760, 0.463845216955300, 0.475045170161378, 0.486284557187706, 0.497556554646881, 0.508854319353835, 0.520170992480304,
		0.531499703718800, 0.542833575453544, 0.554165726935838, 0.565489278461346, 0.576797355546729, 0.588083093103121, 0.599339639603893,
		0.610560161244195, 0.621737846089724, 0.632865908212225, 0.643937591809204, 0.654946175305346, 0.665884975433158, 0.676747351290353,
		0.687526708371517, 0.698216502571607, 0.708810244158849, 0.719301501714632, 0.729683906037996, 0.739951154012344, 0.750097012432047,
		0.760115321786592, 0.770000000000000, 0.779745046123229, 0.789344543977327, 0.798792665745119, 0.808083675509254, 0.817211932734458,
		0.826171895691878, 0.834958124823446, 0.843565286044212, 0.851988153980648, 0.860221615142946, 0.868260671029406, 0.876100441160999,
		0.883736166044290, 0.891163210060907, 0.898377064281809, 0.905373349204632, 0.912147817412476, 0.918696356152490, 0.925014989832710,
		0.931099882435622, 0.936947339846999, 0.942553812098577, 0.947915895523229, 0.953030334821319, 0.957894025036981, 0.962504013443125,
		0.966857501334024, 0.970951845724403, 0.974784560953977, 0.978353320196492, 0.981655956872329, 0.984690465963826, 0.987455005232518,
		0.989947896337551, 0.992167625854595, 0.994112846194642, 0.995782376422118, 0.997175202971826, 0.998290480264278, 0.999127531219040,
		0.999685847665791, 0.999965090652822, 0.999965090652822, 0.999685847665791, 0.999127531219040, 0.998290480264278, 0.997175202971826,
		0.995782376422118, 0.994112846194642, 0.992167625854595, 0.989947896337551, 0.987455005232518, 0.984690465963826, 0.981655956872329,
		0.978353320196492, 0.974784560953977, 0.970951845724403, 0.966857501334024, 0.962504013443125, 0.957894025036981, 0.953030334821319,
		0.947915895523229, 0.942553812098577, 0.936947339846999, 0.931099882435622, 0.925014989832710, 0.918696356152490, 0.912147817412476,
		0.905373349204632, 0.898377064281809, 0.891163210060907, 0.883736166044290, 0.876100441160999, 0.868260671029406, 0.860221615142946,
		0.851988153980648, 0.843565286044212, 0.834958124823446, 0.826171895691878, 0.817211932734458, 0.808083675509254, 0.798792665745119,
		0.789344543977327, 0.779745046123229, 0.770000000000000, 0.760115321786592, 0.750097012432047, 0.739951154012344, 0.729683906037996,
		0.719301501714632, 0.708810244158849, 0.698216502571607, 0.687526708371517, 0.676747351290353, 0.665884975433158, 0.654946175305346,
		0.643937591809204, 0.632865908212225, 0.621737846089724, 0.610560161244195, 0.599339639603893, 0.588083093103121, 0.576797355546729,
		0.565489278461346, 0.554165726935838, 0.542833575453544, 0.531499703718800, 0.520170992480304, 0.508854319353835, 0.497556554646881,
		0.486284557187706, 0.475045170161378, 0.463845216955300, 0.452691497016760, 0.441590781725011, 0.430549810280395, 0.419575285613002,
		0.408673870313343, 0.397852182587524, 0.387116792239357, 0.376474216681854, 0.365930916980537, 0.355493293930949, 0.345167684172755,
		0.334960356342792, 0.324877507269410, 0.314925258210395, 0.305109651136783, 0.295436645064799, 0.285912112438167, 0.276541835562968,
		0.267331503097225, 0.258286706597345, 0.249412937123505, 0.240715581906049, 0.232199921074925, 0.223871124454139, 0.215734248423172,
		0.207794232847276, 0.200055898078497, 0.192523942029263, 0.185202937320297, 0.178097328504598, 0.171211429369169, 0.164549420316138,
		0.158115345824847, 0.151913111996467, 0.145946484182623, 0.140219084699457, 0.134734390628539, 0.129495731705944, 0.124506288300781,
		0.119769089484404, 0.115287011191475, 0.111062774473996, 0.107098943849369, 0.103397925743485, 0.099961967029797, 0.096793153665248,
		0.093893409423896, 0.091264494728999, 0.088908005584270, 0.086825372604947, 0.085017860149273, 0.083486565550910, 0.082232418452750,
		0.081256180242538, 0.080558443590627, 0.080139632090179, 0.080000000000000
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - FlatTopGain")
{
	{
		auto gain = window::calc_amplitude_gain<double, window::flattop<double>>(8);
		double gain_expected = 5.302818796386621;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::flattop<double>>(128);
		double gain_expected = 4.675268693706673;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::flattop<double>>(512);
		double gain_expected = 4.647767218480163;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::flattop<double>>(16384);
		double gain_expected = 4.638955510754467;
		CHECK(gain == Approx(gain_expected));
	}
}

TEST_CASE("Window - FlatTopFloatSmall")
{
	using FloatT = float;
	auto win = create_window<FloatT, window::flattop<FloatT>>(8);

	std::vector<FloatT> expected = {
		-0.000421051000000, -0.036840781154923, 0.010703716716153, 0.780873914938770, 0.780873914938770, 0.010703716716153,
		-0.036840781154923, -0.000421051000000
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - FlatTopDoubleSmall")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::flattop<FloatT>>(8);

	std::vector<FloatT> expected = {
		-0.000421051000000, -0.036840781154923, 0.010703716716153, 0.780873914938770, 0.780873914938770, 0.010703716716153,
		-0.036840781154923, -0.000421051000000
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - FlatTopDoubleLarge")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::flattop<FloatT>>(256);

	std::vector<FloatT> expected = {
		-0.000421051000000, -0.000436659490782, -0.000483663782943, -0.000562598641738, -0.000674349696856, -0.000820144979401,
		-0.001001543085212, -0.001220417976986, -0.001478940442443, -0.001779556231563, -0.002124960902685, -0.002518071415384,
		-0.002961994517409, -0.003459991983935, -0.004015442779792, -0.004631802229465, -0.005312558295298, -0.006061185081706,
		-0.006881093702061, -0.007775580665323, -0.008747773961265, -0.009800577046162, -0.010936610954949, -0.012158154790793,
		-0.013467084868694, -0.014864812815692, -0.016352222956415, -0.017929609338566, -0.019596612778384, -0.021352158330598,
		-0.023194393610757, -0.025120628419562, -0.027127276138673, -0.029209797385042, -0.031362646425778, -0.033579220867505,
		-0.035851815142883, -0.038171578322033, -0.040528476777797, -0.042911262230822, -0.045307445693099, -0.047703277816724,
		-0.050083736137964, -0.052432519685291, -0.054732051393607, -0.056963488735580, -0.059106742944756, -0.061140507163974,
		-0.063042293806821, -0.064788481369425, -0.066354370875188, -0.067714252076212, -0.068841479472678, -0.069708558145464,
		-0.070287239328412, -0.070548625575245, -0.070463285302718, -0.070001376416658, -0.069132778651722, -0.067827234179550,
		-0.066054495964062, -0.063784483267707, -0.060987443639008, -0.057634120640562, -0.053695926508227, -0.049145118867417,
		-0.043954980571641, -0.038100001672458, -0.031556062479395, -0.024300616623639, -0.016312873001061, -0.007573975438819,
		0.001932821094179, 0.012221978928755, 0.023305511992752, 0.035192827167530, 0.047890573898064, 0.061402503233118,
		0.075729337445292,
		0.090868651345027, 0.106814766358197, 0.123558658383638, 0.141087880385248, 0.159386500603226, 0.178435057191136,
		0.198210530000000,
		0.218686330138212, 0.239832307837127, 0.261614779047383, 0.283996571081039, 0.306937087500129, 0.330392392334040,
		0.354315313587027,
		0.378655565873987, 0.403359891898242, 0.428372222360416, 0.453633853763390, 0.479083643455783, 0.504658221136277,
		0.530292215924370,
		0.555918497990583, 0.581468433631835, 0.606872152576212, 0.632058826206813, 0.656956955307250, 0.681494665852601,
		0.705600011299798,
		0.729201279771092, 0.752227304474060, 0.774607775661899, 0.796273552409043, 0.817156972459602, 0.837192158400087,
		0.856315318413451,
		0.874465039888664, 0.891582574188920, 0.907612110921859, 0.922501040106862, 0.936200200697049, 0.948664113986879,
		0.959851200519604,
		0.969723979201831, 0.978249247434398, 0.985398241179102, 0.991146773998595, 0.995475354231399, 0.998369279594438,
		0.999818708640924,
		0.999818708640924, 0.998369279594438, 0.995475354231399, 0.991146773998595, 0.985398241179102, 0.978249247434398,
		0.969723979201831,
		0.959851200519604, 0.948664113986879, 0.936200200697049, 0.922501040106862, 0.907612110921859, 0.891582574188920,
		0.874465039888664,
		0.856315318413451, 0.837192158400087, 0.817156972459602, 0.796273552409043, 0.774607775661899, 0.752227304474060,
		0.729201279771092,
		0.705600011299798, 0.681494665852601, 0.656956955307250, 0.632058826206813, 0.606872152576212, 0.581468433631835,
		0.555918497990583,
		0.530292215924370, 0.504658221136277, 0.479083643455783, 0.453633853763390, 0.428372222360416, 0.403359891898242,
		0.378655565873987,
		0.354315313587027, 0.330392392334040, 0.306937087500129, 0.283996571081039, 0.261614779047383, 0.239832307837127,
		0.218686330138212,
		0.198210530000000, 0.178435057191136, 0.159386500603226, 0.141087880385248, 0.123558658383638, 0.106814766358197,
		0.090868651345027,
		0.075729337445292, 0.061402503233118, 0.047890573898064, 0.035192827167530, 0.023305511992752, 0.012221978928755,
		0.001932821094179,
		-0.007573975438819, -0.016312873001061, -0.024300616623639, -0.031556062479395, -0.038100001672458, -0.043954980571641,
		-0.049145118867417, -0.053695926508227, -0.057634120640562, -0.060987443639008, -0.063784483267707, -0.066054495964062,
		-0.067827234179550, -0.069132778651722, -0.070001376416658, -0.070463285302718, -0.070548625575245, -0.070287239328412,
		-0.069708558145464, -0.068841479472678, -0.067714252076212, -0.066354370875188, -0.064788481369425, -0.063042293806821,
		-0.061140507163974, -0.059106742944756, -0.056963488735580, -0.054732051393607, -0.052432519685291, -0.050083736137964,
		-0.047703277816724, -0.045307445693099, -0.042911262230822, -0.040528476777797, -0.038171578322033, -0.035851815142883,
		-0.033579220867505, -0.031362646425778, -0.029209797385042, -0.027127276138673, -0.025120628419562, -0.023194393610757,
		-0.021352158330598, -0.019596612778384, -0.017929609338566, -0.016352222956415, -0.014864812815692, -0.013467084868694,
		-0.012158154790793, -0.010936610954949, -0.009800577046162, -0.008747773961265, -0.007775580665323, -0.006881093702061,
		-0.006061185081706, -0.005312558295298, -0.004631802229465, -0.004015442779792, -0.003459991983935, -0.002961994517409,
		-0.002518071415384, -0.002124960902685, -0.001779556231563, -0.001478940442443, -0.001220417976986, -0.001001543085212,
		-0.000820144979401, -0.000674349696856, -0.000562598641738, -0.000483663782943, -0.000436659490782, -0.000421051000000
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - BlackmanHarrisGain")
{
	{
		auto gain = window::calc_amplitude_gain<double, window::blackmanharris<double>>(8);
		double gain_expected = 3.185588398087054;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::blackmanharris<double>>(128);
		double gain_expected = 2.809401222221222;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::blackmanharris<double>>(512);
		double gain_expected = 2.792910436871740;
		CHECK(gain == Approx(gain_expected));
	}
	{
		auto gain = window::calc_amplitude_gain<double, window::blackmanharris<double>>(16384);
		double gain_expected = 2.787626560759713;
		CHECK(gain == Approx(gain_expected));
	}
}

TEST_CASE("Window - BlackmanHarrisSmall")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::blackmanharris<FloatT>>(8);

	std::vector<FloatT> expected = {
		0.000060000000000, 0.033391723478151, 0.332833504298565, 0.889369772223284, 0.889369772223284, 0.332833504298565, 0.033391723478151,
		0.000060000000000
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - BlackmanHarrisLarge")
{
	using FloatT = double;
	auto win = create_window<FloatT, window::blackmanharris<FloatT>>(256);

	std::vector<FloatT> expected = {
		0.000060000000000, 0.000068600495788, 0.000094554235665, 0.000138317972118, 0.000200652939687, 0.000282624815874, 0.000385603647113,
		0.000511263720696, 0.000661583358361, 0.000838844602223, 0.001045632759024, 0.001284835764146, 0.001559643322713, 0.001873545781273,
		0.002230332680162, 0.002634090933612, 0.003089202582157, 0.003600342059775, 0.004172472916640, 0.004810843937299, 0.005520984593523,
		0.006308699771149, 0.007180063710738, 0.008141413103047, 0.009199339282006, 0.010360679460109, 0.011632506953998, 0.013022120351347,
		0.014537031574059, 0.016184952797227, 0.017973782188209, 0.019911588435599, 0.022006594043690, 0.024267157374357, 0.026701753424878,
		0.029318953337332, 0.032127402642444, 0.035135798248465, 0.038352864193446, 0.041787326187353, 0.045447884978633, 0.049343188588126,
		0.053481803461536, 0.057872184600001, 0.062522644736567, 0.067441322634521, 0.072636150591555, 0.078114821241521, 0.083884753753060,
		0.089953059531644, 0.096326507538412, 0.103011489345664, 0.110013984054888, 0.117339523208722, 0.124993155833237, 0.132979413751347,
		0.141302277311952, 0.149965141682594, 0.158970783855870, 0.168321330521676, 0.178018226958364, 0.188062207096274, 0.198453264906617,
		0.209190627267519, 0.220272728456991, 0.231697186419891, 0.243460780952324, 0.255559433942627, 0.267988191802991, 0.280741210219888,
		0.293811741344895, 0.307192123540185, 0.320873773784952, 0.334847182840354, 0.349101913261272, 0.363626600333280, 0.378408956002775,
		0.393435775857244, 0.408692949201249, 0.424165472261851, 0.439837464544997, 0.455692188351901, 0.471712071451681, 0.487878732893566,
		0.504173011928894, 0.520575000000000, 0.537064075739880, 0.553618942913444, 0.570217671218166, 0.586837739849117, 0.603456083720800,
		0.620049142225947, 0.636592910399508, 0.653062992344619, 0.669434656766309, 0.685682894448284, 0.701782477498219, 0.717708020177819,
		0.733434041125353, 0.748935026770594, 0.764185495735084, 0.779160064004473, 0.793833510654336, 0.808180843906439, 0.822177367288885,
		0.835798745670991, 0.849021070942092, 0.861820927102799, 0.874175454537554, 0.886062413238582, 0.897460244753653, 0.908348132633259,
		0.918706061157068, 0.928514872124656, 0.937756319501615, 0.946413121719173, 0.954469011433315, 0.961908782558176, 0.968718334398004,
		0.974884712712356, 0.980396147560222, 0.985242087780551, 0.989413231979012, 0.992901555903788, 0.995700336106684, 0.997804169799770,
		0.999208990832118, 0.999912081725875, 0.999912081725875, 0.999208990832118, 0.997804169799770, 0.995700336106684, 0.992901555903788,
		0.989413231979012, 0.985242087780551, 0.980396147560222, 0.974884712712356, 0.968718334398004, 0.961908782558176, 0.954469011433315,
		0.946413121719173, 0.937756319501615, 0.928514872124656, 0.918706061157068, 0.908348132633259, 0.897460244753652, 0.886062413238582,
		0.874175454537554, 0.861820927102799, 0.849021070942092, 0.835798745670992, 0.822177367288886, 0.808180843906440, 0.793833510654336,
		0.779160064004473, 0.764185495735084, 0.748935026770594, 0.733434041125354, 0.717708020177819, 0.701782477498220, 0.685682894448284,
		0.669434656766309, 0.653062992344619, 0.636592910399509, 0.620049142225948, 0.603456083720801, 0.586837739849117, 0.570217671218166,
		0.553618942913444, 0.537064075739880, 0.520575000000000, 0.504173011928894, 0.487878732893566, 0.471712071451682, 0.455692188351902,
		0.439837464544998, 0.424165472261852, 0.408692949201249, 0.393435775857244, 0.378408956002775, 0.363626600333280, 0.349101913261272,
		0.334847182840354, 0.320873773784953, 0.307192123540185, 0.293811741344895, 0.280741210219888, 0.267988191802991, 0.255559433942627,
		0.243460780952324, 0.231697186419892, 0.220272728456991, 0.209190627267519, 0.198453264906618, 0.188062207096274, 0.178018226958365,
		0.168321330521677, 0.158970783855871, 0.149965141682594, 0.141302277311952, 0.132979413751347, 0.124993155833237, 0.117339523208722,
		0.110013984054888, 0.103011489345664, 0.096326507538412, 0.089953059531644, 0.083884753753060, 0.078114821241521, 0.072636150591555,
		0.067441322634521, 0.062522644736567, 0.057872184600001, 0.053481803461536, 0.049343188588126, 0.045447884978633, 0.041787326187353,
		0.038352864193446, 0.035135798248465, 0.032127402642444, 0.029318953337332, 0.026701753424878, 0.024267157374357, 0.022006594043690,
		0.019911588435598, 0.017973782188209, 0.016184952797227, 0.014537031574059, 0.013022120351347, 0.011632506953998, 0.010360679460109,
		0.009199339282006, 0.008141413103047, 0.007180063710738, 0.006308699771149, 0.005520984593523, 0.004810843937299, 0.004172472916640,
		0.003600342059775, 0.003089202582157, 0.002634090933612, 0.002230332680162, 0.001873545781273, 0.001559643322713, 0.001284835764146,
		0.001045632759024, 0.000838844602223, 0.000661583358361, 0.000511263720696, 0.000385603647113, 0.000282624815874, 0.000200652939687,
		0.000138317972118, 0.000094554235665, 0.000068600495788, 0.000060000000000
	};

	REQUIRE(win.size() == expected.size());
	for (std::size_t i = 0; i < win.size(); i++) { CHECK(win[i] == Approx(expected[i])); }
}

TEST_CASE("Window - Buffer")
{
	const std::vector<double> expected_8 = {
		0, 1. / 3.5, 2. / 3.5, 3. / 3.5, 4. / 3.5, 5. / 3.5, 6. / 3.5, 7. / 3.5
	};
	const std::vector<double> expected_16 = {
		0, 1. / 7.5, 2. / 7.5, 3. / 7.5, 4. / 7.5, 5. / 7.5, 6. / 7.5, 7. / 7.5, 8. / 7.5, 9. / 7.5, 10. / 7.5, 11. / 7.5, 12. / 7.5,
		13. / 7.5, 14. / 7.5, 15. / 7.5
	};

	using buffer = window::buffer<double, window::linear>;
	REQUIRE(buffer::size() == 0);

	buffer::prepare(expected_8.size());
	REQUIRE(buffer::size() == 8);

	buffer::prepare(expected_8.size());  // test seconds time
	REQUIRE(buffer::size() == 8);

	buffer::prepare(expected_16.size());
	REQUIRE(buffer::size() == 24);

	for (std::size_t k = 0; k < expected_8.size(); k++)
	{
		CHECK(buffer::get_window(expected_8.size(), k) == Approx(expected_8[k]));
	}

	for (std::size_t k = 0; k < expected_16.size(); k++)
	{
		CHECK(buffer::get_window(expected_16.size(), k) == Approx(expected_16[k]));
	}

	buffer::clear();
	REQUIRE(buffer::size() == 0);
}

#endif
