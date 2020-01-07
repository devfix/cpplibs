//
// Created by core on 12/27/19.
//


#pragma once

#include "inputstream.h"
#include "iotypes.h"

namespace devfix::base::io
{

	/**
	 * \brief Adapter class to create an <i>inputstream</i> from function pointers.
	 */
	struct source : public inputstream
	{

		/**
		 * \brief Create an <i>inputstream</i> from function pointers to the member the functions of the interface.
		 * \param read function pointer to implementation of read()
		 * \param skip function pointer to implementation of skip()
		 * \param available function pointer to implementation of available()
		 * \param close function pointer to implementation of close()
		 * \param is_closed function pointer to implementation of is_closed()
		 */
		source(
			read_t read,
			skip_t skip,
			available_t available,
			close_t close = DEFAULT_CLOSE,
			is_closed_t is_closed = DEFAULT_IS_CLOSED
		);

		void read(void* buf, std::size_t len) override;

		void skip(std::size_t n) override;

		[[nodiscard]] std::size_t available() override;

		void close() override;

		[[nodiscard]]  bool is_closed() override;

	private:
		const read_t read_;
		const skip_t skip_;
		const available_t available_;
		const close_t close_;
		const is_closed_t is_closed_;

	};

} // namespace devfix::base::io
