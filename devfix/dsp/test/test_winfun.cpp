//
// Created by core on 7/15/20.
//


#if CPPLIBS_ENABLE_DSP_TESTS == 1

#include <catch2/catch.hpp>
#include <iostream>
#include "../../../testutil.h"
#include "../window.h"
#include "../winfun.h"

using namespace devfix::base;
using namespace devfix::dsp;

template<typename FloatT>
std::vector<FloatT> get_win_vals(const window<FloatT>& win)
{
	std::vector<FloatT> vec(win.size(), FloatT(1));
	win.apply(vec);
	return vec;
}

TEST_CASE("devfix/dsp/winfun/hanning/small")
{
	auto test = [] < typename FloatT > (FloatT
	_, long double
	margin)
	{
		const std::vector<FloatT> expected = {
			0, 0.188255099070633, 0.611260466978157, 0.950484433951210, 0.950484433951210, 0.611260466978157, 0.188255099070633, 0
		};
		window<FloatT> win(winfun<FloatT>::hanning, expected.size(), false);
		auto vals = get_win_vals(win);
		testutil::check_equals(vals, expected, margin);
	};
	test((float) (1), testutil::MARGIN_COARSE);
	test((double) (1), testutil::MARGIN_FINE);
	test((long double) (1), testutil::MARGIN_FINE);
}

TEST_CASE("devfix/dsp/winfun/hanning/large")
{
	auto test = [] < typename FloatT > (FloatT
	_, long double
	margin)
	{
		const std::vector<FloatT> expected = {
			0, 0.000151774011064, 0.000607003902855, 0.001365413307106, 0.002426541796468, 0.003789745164032, 0.005454195814427,
			0.007418883266247, 0.009682614765511, 0.012244016009782, 0.015101531982495, 0.018253427897008, 0.021697790249780,
			0.025432527982049, 0.029455373749314, 0.033763885297822, 0.038355446947255, 0.043227271178700, 0.048376400326936,
			0.053799708376026, 0.059493902857108, 0.065455526847236, 0.071680961068069, 0.078166426083117, 0.084907984592225,
			0.091901543821889, 0.099142858009967, 0.106627530983258, 0.114351018826410, 0.122308632640503, 0.130495541389670,
			0.138906774833995, 0.147537226546926, 0.156381657015369, 0.165434696820571, 0.174690849897879, 0.184144496873374,
			0.193789898475375, 0.203621199018723, 0.213632429959747, 0.223817513519747, 0.234170266374782, 0.244684403409547,
			0.255353541533038, 0.266171203553707, 0.277130822111731, 0.288225743666038, 0.299449232533641, 0.310794474978845,
			0.322254583349841, 0.333822600260170, 0.345491502812526, 0.357254206862329, 0.369103571318480, 0.381032402478691,
			0.393033458396751, 0.405099453279087, 0.417223061907935, 0.429396924088454, 0.441613649117072, 0.453865820268349,
			0.466145999297646, 0.478446730956852, 0.490760547520435, 0.503079973319069, 0.515397529278085, 0.527705737457985,
			0.539997125594271, 0.552264231633827, 0.564499608265101, 0.576695827439343, 0.588845484880134, 0.600941204578505,
			0.612975643270874, 0.624941494897115, 0.636831495036041, 0.648638425315601, 0.660355117795127, 0.671974459316964,
			0.683489395824836, 0.694892936646340, 0.706178158736952, 0.717338210882982, 0.728366317860920, 0.739255784550643,
			0.750000000000000, 0.760592441438293, 0.771026678236225, 0.781296375809912, 0.791395299466580, 0.801317318189628,
			0.811056408360737, 0.820606657416789, 0.829962267439361, 0.839117558674617, 0.848066972981463, 0.856805077205876,
			0.865326566479347, 0.873626267439445, 0.881699141370551, 0.889540287262835, 0.897144944787643, 0.904508497187474,
			0.911626474078794, 0.918494554165989, 0.925108567864807, 0.931464499833695, 0.937558491411496, 0.943386842960031,
			0.948946016110129, 0.954232635909762, 0.959243492872962, 0.963975544928287, 0.968425919265655, 0.972591914080410,
			0.976471000213578, 0.980060822687314, 0.983359202134594, 0.986364136122302, 0.989073800366903, 0.991486549841951,
			0.993600919776785, 0.995415626545780, 0.996929568447637, 0.998141826374215, 0.999051664368522, 0.999658530071512,
			0.999962055057415, 0.999962055057415, 0.999658530071512, 0.999051664368522, 0.998141826374215, 0.996929568447637,
			0.995415626545780, 0.993600919776785, 0.991486549841951, 0.989073800366903, 0.986364136122302, 0.983359202134594,
			0.980060822687314, 0.976471000213578, 0.972591914080410, 0.968425919265655, 0.963975544928287, 0.959243492872962,
			0.954232635909762, 0.948946016110129, 0.943386842960031, 0.937558491411496, 0.931464499833695, 0.925108567864807,
			0.918494554165989, 0.911626474078794, 0.904508497187474, 0.897144944787643, 0.889540287262835, 0.881699141370551,
			0.873626267439445, 0.865326566479347, 0.856805077205876, 0.848066972981463, 0.839117558674617, 0.829962267439361,
			0.820606657416789, 0.811056408360737, 0.801317318189628, 0.791395299466580, 0.781296375809912, 0.771026678236225,
			0.760592441438293, 0.750000000000000, 0.739255784550643, 0.728366317860920, 0.717338210882982, 0.706178158736952,
			0.694892936646340, 0.683489395824836, 0.671974459316964, 0.660355117795127, 0.648638425315601, 0.636831495036041,
			0.624941494897115, 0.612975643270874, 0.600941204578505, 0.588845484880134, 0.576695827439343, 0.564499608265101,
			0.552264231633827, 0.539997125594271, 0.527705737457985, 0.515397529278085, 0.503079973319069, 0.490760547520435,
			0.478446730956852, 0.466145999297646, 0.453865820268349, 0.441613649117072, 0.429396924088454, 0.417223061907935,
			0.405099453279087, 0.393033458396751, 0.381032402478691, 0.369103571318480, 0.357254206862329, 0.345491502812526,
			0.333822600260170, 0.322254583349841, 0.310794474978845, 0.299449232533641, 0.288225743666038, 0.277130822111731,
			0.266171203553707, 0.255353541533038, 0.244684403409547, 0.234170266374782, 0.223817513519747, 0.213632429959747,
			0.203621199018723, 0.193789898475375, 0.184144496873374, 0.174690849897879, 0.165434696820571, 0.156381657015369,
			0.147537226546926, 0.138906774833995, 0.130495541389670, 0.122308632640503, 0.114351018826410, 0.106627530983258,
			0.099142858009967, 0.091901543821889, 0.084907984592225, 0.078166426083117, 0.071680961068069, 0.065455526847236,
			0.059493902857108, 0.053799708376026, 0.048376400326936, 0.043227271178700, 0.038355446947255, 0.033763885297822,
			0.029455373749314, 0.025432527982049, 0.021697790249780, 0.018253427897008, 0.015101531982495, 0.012244016009782,
			0.009682614765511, 0.007418883266247, 0.005454195814427, 0.003789745164032, 0.002426541796468, 0.001365413307106,
			0.000607003902855, 0.000151774011064, 0
		};
		window<FloatT> win(winfun<FloatT>::hanning, expected.size(), false);
		auto vals = get_win_vals(win);
		testutil::check_equals(vals, expected, margin);
	};
	test((float) (1), testutil::MARGIN_COARSE);
	test((double) (1), testutil::MARGIN_FINE);
	test((long double) (1), testutil::MARGIN_FINE);
}

TEST_CASE("devfix/dsp/winfun/flattop_matlab/small")
{
	auto test = [] < typename FloatT > (FloatT
	_, long double
	margin)
	{
		const std::vector<FloatT> expected = {
			-0.000421051000000, -0.036840781154923, 0.010703716716153, 0.780873914938770, 0.780873914938770, 0.010703716716153,
			-0.036840781154923, -0.000421051000000
		};
		window<FloatT> win(winfun<FloatT>::flattop_matlab, expected.size(), false);
		auto vals = get_win_vals(win);
		testutil::check_equals(vals, expected, margin);
	};
	test((float) (1), testutil::MARGIN_COARSE);
	test((double) (1), testutil::MARGIN_FINE);
	test((long double) (1), testutil::MARGIN_FINE);
}

TEST_CASE("devfix/dsp/winfun/flattop_matlab/large")
{
	auto test = [] < typename FloatT > (FloatT
	_, long double
	margin)
	{
		const std::vector<FloatT> expected = {
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
			0.075729337445292, 0.090868651345027, 0.106814766358197, 0.123558658383638, 0.141087880385248, 0.159386500603226,
			0.178435057191136, 0.198210530000000, 0.218686330138212, 0.239832307837127, 0.261614779047383, 0.283996571081039,
			0.306937087500129, 0.330392392334040, 0.354315313587027, 0.378655565873987, 0.403359891898242, 0.428372222360416,
			0.453633853763390, 0.479083643455783, 0.504658221136277, 0.530292215924370, 0.555918497990583, 0.581468433631835,
			0.606872152576212, 0.632058826206813, 0.656956955307250, 0.681494665852601, 0.705600011299798, 0.729201279771092,
			0.752227304474060, 0.774607775661899, 0.796273552409043, 0.817156972459602, 0.837192158400087, 0.856315318413451,
			0.874465039888664, 0.891582574188920, 0.907612110921859, 0.922501040106862, 0.936200200697049, 0.948664113986879,
			0.959851200519604, 0.969723979201831, 0.978249247434398, 0.985398241179102, 0.991146773998595, 0.995475354231399,
			0.998369279594438, 0.999818708640924, 0.999818708640924, 0.998369279594438, 0.995475354231399, 0.991146773998595,
			0.985398241179102, 0.978249247434398, 0.969723979201831, 0.959851200519604, 0.948664113986879, 0.936200200697049,
			0.922501040106862, 0.907612110921859, 0.891582574188920, 0.874465039888664, 0.856315318413451, 0.837192158400087,
			0.817156972459602, 0.796273552409043, 0.774607775661899, 0.752227304474060, 0.729201279771092, 0.705600011299798,
			0.681494665852601, 0.656956955307250, 0.632058826206813, 0.606872152576212, 0.581468433631835, 0.555918497990583,
			0.530292215924370, 0.504658221136277, 0.479083643455783, 0.453633853763390, 0.428372222360416, 0.403359891898242,
			0.378655565873987, 0.354315313587027, 0.330392392334040, 0.306937087500129, 0.283996571081039, 0.261614779047383,
			0.239832307837127, 0.218686330138212, 0.198210530000000, 0.178435057191136, 0.159386500603226, 0.141087880385248,
			0.123558658383638, 0.106814766358197, 0.090868651345027, 0.075729337445292, 0.061402503233118, 0.047890573898064,
			0.035192827167530, 0.023305511992752, 0.012221978928755, 0.001932821094179, -0.007573975438819, -0.016312873001061,
			-0.024300616623639, -0.031556062479395, -0.038100001672458, -0.043954980571641, -0.049145118867417, -0.053695926508227,
			-0.057634120640562, -0.060987443639008, -0.063784483267707, -0.066054495964062, -0.067827234179550, -0.069132778651722,
			-0.070001376416658, -0.070463285302718, -0.070548625575245, -0.070287239328412, -0.069708558145464, -0.068841479472678,
			-0.067714252076212, -0.066354370875188, -0.064788481369425, -0.063042293806821, -0.061140507163974, -0.059106742944756,
			-0.056963488735580, -0.054732051393607, -0.052432519685291, -0.050083736137964, -0.047703277816724, -0.045307445693099,
			-0.042911262230822, -0.040528476777797, -0.038171578322033, -0.035851815142883, -0.033579220867505, -0.031362646425778,
			-0.029209797385042, -0.027127276138673, -0.025120628419562, -0.023194393610757, -0.021352158330598, -0.019596612778384,
			-0.017929609338566, -0.016352222956415, -0.014864812815692, -0.013467084868694, -0.012158154790793, -0.010936610954949,
			-0.009800577046162, -0.008747773961265, -0.007775580665323, -0.006881093702061, -0.006061185081706, -0.005312558295298,
			-0.004631802229465, -0.004015442779792, -0.003459991983935, -0.002961994517409, -0.002518071415384, -0.002124960902685,
			-0.001779556231563, -0.001478940442443, -0.001220417976986, -0.001001543085212, -0.000820144979401, -0.000674349696856,
			-0.000562598641738, -0.000483663782943, -0.000436659490782, -0.000421051000000
		};
		window<FloatT> win(winfun<FloatT>::flattop_matlab, expected.size(), false);
		auto vals = get_win_vals(win);
		testutil::check_equals(vals, expected, margin);
	};
	test((float) (1), testutil::MARGIN_COARSE);
	test((double) (1), testutil::MARGIN_FINE);
	test((long double) (1), testutil::MARGIN_FINE);
}

#endif