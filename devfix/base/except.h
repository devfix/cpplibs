//
// Created by core on 7/9/20.
//

#pragma once

#include <string>
#include <tuple>
#include <optional>
#include <queue>
#include "error/baseexception.h"
#include "error/interruptedexception.h"
#include "error/ioexception.h"
#include "error/timeoutexception.h"


namespace devfix::base
{

	struct except
	{
		using trace_t = std::queue<std::optional<std::pair<std::string, std::string>>>;

		[[nodiscard]] static trace_t get_trace();

	private:
		static void unwind_nested(trace_t& trace);
	};

}
