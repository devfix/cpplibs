//
// Created by core on 12/27/19.
//


#pragma once

#include "outputstream.h"
#include "iotypes.h"

namespace devfix::base::io
{

	struct sink : public outputstream
	{

		sink(write_t write, flush_t flush, close_t close = DEFAULT_CLOSE, is_closed_t is_closed = DEFAULT_IS_CLOSED);

		void write(const void* buf, std::size_t len);

		void flush();

		void close();

		[[nodiscard]] bool is_closed();

	private:
		const write_t write_;
		const flush_t flush_;
		const close_t close_;
		const is_closed_t is_closed_;

	};

} // namespace devfix::base::io
