//
// Created by core on 7/17/19.
//

#pragma once

#include <sstream>
#include <variant>
#include <functional>
#include "strutil.h"

namespace devfix::base
{

	template<
		class CharT,
		class Traits = std::char_traits<CharT>,
		class Allocator = std::allocator<CharT>
	>
	struct strout : public std::basic_stringbuf<CharT, Traits, Allocator>
	{
		using pipe_fun_t = std::function<void(const std::basic_string<CharT>&)>;
		using flush_fun_t = std::function<void()>;
		using pair_t = std::pair<pipe_fun_t, flush_fun_t>;
		using stream_t = std::basic_ostream<CharT>;
		using pipes_t = std::vector<std::variant<stream_t*, pair_t>>;

		explicit strout(pipes_t pipes) : pipes_(std::move(pipes)) {}

		~strout()
		{
			if (enabled_ && (!prefixed_ || buffer_.str().length() != prefix_.length())) { sync(); }
		}

	private:
		void write_buffer()
		{
			for (auto& pipe : pipes_)
			{
				if (std::holds_alternative<stream_t*>(pipe)) { (*std::get<stream_t*>(pipe)) << buffer_.str(); }
				else { std::get<pair_t>(pipe).first(buffer_.str()); }
			}
		}

		void clear_buffer() { buffer_.str(MULTISTRING(CharT, "")); }

		void flush()
		{
			for (auto& pipe: pipes_)
			{
				if (std::holds_alternative<stream_t*>(pipe)) { std::get<stream_t*>(pipe)->flush(); }
				else { std::get<pair_t>(pipe).second(); }
			}
		}

	protected:
		using int_type = typename std::basic_stringbuf<CharT, Traits, Allocator>::int_type;

		int sync() override
		{
			std::basic_string<CharT> str = buffer_.str();
			if (enabled_ && (!prefixed_ || str.length() != prefix_.length()))
			{
				write_buffer();
				clear_buffer();
				prefixed_ = false;
			}

			// always flush streams
			flush();

			return 0; // always successful
		}

		int_type overflow(int_type c) override
		{
			if (enabled_)
			{
				buffer_ << static_cast<CharT>(c);

				if (c == static_cast<CharT>('\n'))
				{
					write_buffer();
					clear_buffer();
					buffer_ << prefix_;
					prefixed_ = true;
				}
				else if (c == STX)
				{
					buffer_.str(CLEAR_LINE.data());  // set buffer

					write_buffer();
					clear_buffer();
					buffer_ << prefix_;
					prefixed_ = true;
				}
			}
			return 0; // always successful
		}

	public:
		void set_enabled(bool enabled) { enabled_ = enabled; }

		bool is_enabled() const { return enabled_; }

		void set_prefix(const std::basic_string<CharT>& prefix)
		{
			if (prefixed_ && (buffer_.str().length() == prefix_.length()))
			{
				clear_buffer();
				buffer_ << prefix;
				prefixed_ = true;
			}
			prefix_ = prefix;
		}

		const std::basic_string<CharT>& get_prefix() const { return prefix_; }

		static constexpr CharT STX = static_cast<CharT>('\x02'); //!< Start of Text, clear whole line before new text gets displayed

	protected:
		bool enabled_ = true;
		std::basic_string<CharT> prefix_;
		std::basic_stringstream<CharT> buffer_;
		std::vector<std::variant<stream_t*, pair_t>> pipes_;
		bool prefixed_ = true;

		static constexpr std::basic_string_view<CharT> CLEAR_LINE = MULTISTRING(CharT, "\033[2K\r");
	};

}
