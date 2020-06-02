//
// Created by core on 7/17/19.
//

#pragma once

#include <sstream>
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
		explicit strout(std::basic_ostream<CharT>& output_stream) : output_stream_(output_stream) {}

		~strout()
		{
			if (enabled_ && (!prefixed_ || buffer_.str().length() != prefix_.length()))
			{
				output_stream_ << buffer_.str() << std::flush;
			}
		}

	protected:
		using int_type = typename std::basic_stringbuf<CharT, Traits, Allocator>::int_type;

		int sync() override
		{
			if (enabled_ && (!prefixed_ || buffer_.str().length() != prefix_.length()))
			{
				output_stream_ << buffer_.str() << std::flush;
				buffer_.str(MULTISTRING(CharT, ""));  // clear buffer
				prefixed_ = false;
			}
			return 0; // always successful
		}

		int_type overflow(int_type c) override
		{
			if (enabled_)
			{
				buffer_ << static_cast<CharT>(c);

				if (c == static_cast<CharT>('\n'))
				{
					output_stream_ << buffer_.str();
					buffer_.str(MULTISTRING(CharT, ""));  // clear buffer
					buffer_ << prefix_;
					prefixed_ = true;
				}
				else if (c == STX)
				{
					output_stream_ << CLEAR_LINE;
					buffer_.str(MULTISTRING(CharT, ""));  // clear buffer
					buffer_ << prefix_;
					prefixed_ = true;
				}
			}
			return 0; // always successful
		}

	public:
		void set_enabled(bool enabled)
		{
			enabled_ = enabled;
		}

		bool is_enabled() const
		{
			return enabled_;
		}

		void set_prefix(const std::basic_string<CharT>& prefix)
		{
			if (prefixed_ && (buffer_.str().length() == prefix_.length()))
			{
				buffer_.str(MULTISTRING(CharT, ""));  // clear buffer
				buffer_ << prefix;
				prefixed_ = true;
			}
			prefix_ = prefix;
		}

		const std::basic_string<CharT>& get_prefix() const
		{
			return prefix_;
		}

		static constexpr CharT STX = static_cast<CharT>('\x02'); //!< Start of Text, clear whole line before new text gets displayed

	protected:
		bool enabled_ = true;
		std::basic_string<CharT> prefix_;
		std::basic_stringstream<CharT> buffer_;
		std::basic_ostream<CharT>& output_stream_;
		bool prefixed_ = true;

		static constexpr std::basic_string_view<CharT> CLEAR_LINE = MULTISTRING(CharT, "\033[2K\r");
	};

}
