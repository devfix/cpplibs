//
// Created by core on 7/9/20.
//

#include "except.h"
#include "type.h"

namespace devfix::base
{
	except::trace_t except::get_trace()
	{
		trace_t trace;
		unwind_nested(trace);
		return trace;
	}

	void except::unwind_nested(devfix::base::except::trace_t& trace)
	{
		try { throw; }
		catch (...)
		{
			try { throw; }
			catch (const std::exception& e)
			{
				trace.emplace(std::make_pair(type::remove_nested(type::demangle(typeid(e).name())), e.what()));
			}
			catch (...) { trace.emplace(); }
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
