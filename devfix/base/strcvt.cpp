//
// Created by core on 7/7/20.
//

#include <codecvt>
#include <type_traits>
#include "strcvt.h"

// utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
template<class Facet>
struct deletable_facet : Facet
{
	template<class ...Args>
	explicit deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}

	~deletable_facet() override = default;
};

using c16c08 =  std::wstring_convert<deletable_facet<std::codecvt<char16_t, char08_t, std::mbstate_t>>, char16_t>;
using c32c08 =  std::wstring_convert<deletable_facet<std::codecvt<char32_t, char08_t, std::mbstate_t>>, char32_t>;

namespace devfix::base
{

	strcvt::u16string strcvt::c08to16(const u08string& s)
	{
		return c16c08().from_bytes(s);
	}

	strcvt::u32string strcvt::c08to32(const u08string& s)
	{
		return c32c08().from_bytes(s);
	}

	strcvt::u08string strcvt::c16to08(const u16string& s)
	{
		return c16c08().to_bytes(s);
	}

	strcvt::u32string strcvt::c16to32(const u16string& s)
	{
		return c08to32(c16to08(s));
	}

	strcvt::u08string strcvt::c32to08(const u32string& s)
	{
		return c32c08().to_bytes(s);
	}

	strcvt::u16string strcvt::c32to16(const u32string& s)
	{
		return c08to16(c32to08(s));
	}
}
