//
// Created by core on 6/9/20.
//

#pragma once

#include <sstream>
#include <functional>

namespace devfix::base
{
	template<
		class CharT,
		class Traits = std::char_traits<CharT>,
		class Allocator = std::allocator<CharT>
	>
	struct strpipe : public std::basic_stringbuf<CharT, Traits, Allocator>
	{
		using pipe_fun_t = std::function<void(CharT)>;

		strpipe(pipe_fun_t pipe_fun) : pipe_fun_(pipe_fun) {}

		~strpipe() = default;

	protected:
		using int_type = typename std::basic_stringbuf<CharT, Traits, Allocator>::int_type;

		int sync() override { return 0; }

		int_type overflow(int_type c) override
		{
			pipe_fun_(static_cast<CharT>(c));
			return 0; // always successful
		}

	protected:
		pipe_fun_t pipe_fun_;
	};

}
