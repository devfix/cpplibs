//
// Created by core on 7/9/20.
//

#pragma once

#include <string>
#include <tuple>
#include <optional>
#include <list>
#include "error/baseexception.h"
#include "error/interruptedexception.h"
#include "error/ioexception.h"
#include "error/timeoutexception.h"


namespace devfix::base
{

	struct except
	{
		except() = delete;

		struct trace : public std::list<std::optional<std::pair<std::string, std::string>>>
		{
			[[nodiscard]] operator std::string() const
			{
				std::size_t depth = 0;
				std::string str;
				for (auto it = begin(); it != end(); it++, depth++)
				{
					if (depth) { str += "\n" + std::string(depth * 2 - 2, ' ') + "\u2570\u2574"; }
					str += it->has_value() ? ((**it).first + ": " + (**it).second) : "[...]";
				}
				return str;
			}

			[[nodiscard]] std::string to_string() { return operator std::string(); }
		};

		[[nodiscard]] static trace get_trace();

	private:
		static void unwind_nested(trace& trace);
	};

}
