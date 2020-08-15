//
// Created by core on 8/15/20.
//

#pragma once

#include <type_traits>

namespace devfix::base::numbers
{
	template<typename FloatT>
	constexpr FloatT e_v = static_cast<FloatT>(2.718281828459045235360287471352662498L);
	constexpr double e = e_v<double>;

	template<typename FloatT>
	constexpr FloatT log2e_v = static_cast<FloatT>(1.442695040888963407359924681001892137L);
	constexpr double log2e = log2e_v<double>;

	template<typename FloatT>
	constexpr FloatT log10e_v = static_cast<FloatT>(0.434294481903251827651128918916605082L);
	constexpr double log10e = log10e_v<double>;

	template<typename FloatT>
	constexpr FloatT ln2_v = static_cast<FloatT>(0.693147180559945309417232121458176568L);
	constexpr double ln2 = ln2_v<double>;

	template<typename FloatT>
	constexpr FloatT ln10_v = static_cast<FloatT>(2.302585092994045684017991454684364208L);
	constexpr double ln10 = ln10_v<double>;

	template<typename FloatT>
	constexpr FloatT pi_v = static_cast<FloatT>(3.141592653589793238462643383279502884L);
	constexpr double pi = pi_v<double>;

	template<typename FloatT>
	constexpr FloatT inv_pi_v = static_cast<FloatT>(0.318309886183790671537767526745028724L);
	constexpr double inv_pi = inv_pi_v<double>;

	template<typename FloatT>
	constexpr FloatT sqrt2_v = static_cast<FloatT>(1.414213562373095048801688724209698079L);
	constexpr double sqrt2 = sqrt2_v<double>;
}
