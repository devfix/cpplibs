//
// Created by core on 12/27/19.
//


#pragma once

#include "outputstream.h"
#include "iotypes.h"

namespace devfix::base::io
{

	/**
	 * \brief Adapter class to create an <i>outputstream</i> from function pointers.
	 */
	struct sink : public outputstream
	{

		/**
		 * \brief Create an <i>outputstream</i> from function pointers to the member the functions of the interface.
		 * \param write function pointer to implementation of write()
		 * \param flush function pointer to implementation of flush()
		 * \param close function pointer to implementation of close()
		 * \param is_closed function pointer to implementation of is_closed()
		 */
		sink(
			write_t write,
			flush_t flush,
			close_t close = DEFAULT_CLOSE,
			is_closed_t is_closed = DEFAULT_IS_CLOSED
		);

		void write(const void* buf, std::size_t len) override;

		void flush() override;

		void close() override;

		[[nodiscard]] bool is_closed() override;

	private:
		const write_t write_;
		const flush_t flush_;
		const close_t close_;
		const is_closed_t is_closed_;

	};

} // namespace devfix::base::io
