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
	protected:
		using int_type = typename std::basic_stringbuf<CharT, Traits, Allocator>::int_type;

	public:
		explicit strout(std::ostream& output_stream) : output_stream_(output_stream) {}

		~strout()
		{
			if (enabled_ && (!prefixed_ || buffer_.str().length() != prefix_.length()))
			{
				output_stream_ << buffer_.str() << std::flush;
			}
		}

		int sync() override
		{
			if (enabled_ && (!prefixed_ || buffer_.str().length() != prefix_.length()))
			{
				output_stream_ << buffer_.str() << std::flush;
				buffer_ = std::stringstream();
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
					buffer_ = std::stringstream();
					buffer_ << prefix_;
					prefixed_ = true;
				}
				else if (c == STX)
				{
					output_stream_ << CLEAR_LINE;
					buffer_ = std::stringstream();
					buffer_ << prefix_;
					prefixed_ = true;
				}
			}
			return 0; // always successful
		}

		void set_enabled(bool enabled)
		{
			enabled_ = enabled;
		}

		bool is_enabled() const
		{
			return enabled_;
		}

		void set_prefix(const std::string& prefix)
		{
			std::string prefix_old = prefix_;
			prefix_ = prefix;
			if (prefixed_ && (buffer_.str().length() == prefix_old.length()))
			{
				buffer_ = std::stringstream();
				buffer_ << prefix;
				prefixed_ = true;
			}
		}

		std::string get_prefix()
		{
			return prefix_;
		}

		static constexpr CharT STX = MULTICHAR(CharT, '\x02'); //!< Start of Text, clear whole line before new text gets displayed

	protected:
		bool enabled_ = true;
		std::string prefix_;
		std::stringstream buffer_;
		std::ostream& output_stream_;
		bool prefixed_ = true;

		static constexpr std::basic_string_view<CharT> CLEAR_LINE = MULTISTRING(CharT, "\033[2K\r");
	};

}
