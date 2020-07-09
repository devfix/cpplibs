//
// Created by core on 7/9/20.
//

#include "except.h"
#include "type.h"

namespace devfix::base
{
	except::trace except::get_trace()
	{
		trace trace;
		unwind_nested(trace);
		return trace;
	}

	void except::unwind_nested(devfix::base::except::trace& trace)
	{
		try { throw; }
		catch (...)
		{
			try { throw; }
			catch (const std::exception& e)
			{
				trace.emplace_back(std::make_pair(type::remove_nested(type::demangle(typeid(e).name())), e.what()));
			}
			catch (...) { trace.emplace_back(); }
		}
		try { throw; }
		catch (const std::nested_exception& nested)
		{
			try { nested.rethrow_nested(); }
			catch (...) { unwind_nested(trace); }
		}
		catch (...) {}
	}
}
