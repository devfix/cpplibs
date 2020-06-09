//
// Created by core on 6/9/20.
//

#pragma once

#include <sstream>
#include <functional>
#include <utility>

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
		using flush_fun_t = std::function<void()>;

		strpipe(pipe_fun_t pipe_fun, flush_fun_t flush_fun) : pipe_fun_(std::move(pipe_fun)), flush_fun_(std::move(flush_fun)) {}

		~strpipe() = default;

	protected:
		using int_type = typename std::basic_stringbuf<CharT, Traits, Allocator>::int_type;

		int sync() override
		{
			flush_fun_();
			return 0; // always successful
		}

		int_type overflow(int_type c) override
		{
			pipe_fun_(static_cast<CharT>(c));
			return 0; // always successful
		}

	protected:
		pipe_fun_t pipe_fun_;
		flush_fun_t flush_fun_;
	};

}
