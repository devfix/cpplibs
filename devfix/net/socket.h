//
// Created by core on 12/18/19.
//


#pragma once

#include "inetaddress.h"
#include "../base/memory.h"
#include "../base/platform.h"
#include "../base/io/inputstream.h"
#include "../base/io/outputstream.h"

namespace devfix::net
{

	struct socket
	{
		typedef std::uint32_t timeout_t;
		static constexpr timeout_t DEFAULT_TIMEOUT = 3000; //!< default read timeout in milliseconds
		static constexpr timeout_t
			DEFAULT_READ_BLOCKING_TIME = 100; //!< default read timeout until refresh in milliseconds

		virtual ~socket() = default;

		[[nodiscard]] virtual const inetaddress& get_local_address() const noexcept = 0;
		[[nodiscard]] virtual const inetaddress& get_remote_address() const noexcept = 0;

		[[nodiscard]] virtual base::io::inputstream& get_inputstream() const noexcept = 0;
		[[nodiscard]] virtual base::io::outputstream& get_outputstream() const noexcept = 0;

		/**
		 * Set the socket as interrupted.
		 * @param interrupted If set true, any read call returns after the read blocking time expired and throws an error.
		 */
		virtual void set_interrupted(bool interrupted) noexcept = 0;

		/**
		 * @returns true if the socket is interrupted.
		 */
		[[nodiscard]] virtual bool interrupted() const noexcept = 0;

		virtual void set_timeout(timeout_t timeout) noexcept = 0;
		[[nodiscard]] virtual timeout_t get_timeout() const noexcept = 0;

	};

} // namespace devfix::net
