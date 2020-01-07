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

	/**
	 * \brief This class implements client sockets (also called just "sockets").
	 *
	 * A socket is an endpoint for communication between two machines.
	 * The actual work of the socket is performed by an instance of the platform specific implementation class.
	 */
	struct socket
	{
		typedef std::uint32_t timeout_t;
		static constexpr timeout_t DEFAULT_TIMEOUT = 3000; //!< default read timeout in milliseconds
		static constexpr timeout_t
			DEFAULT_READ_BLOCKING_TIME = 100; //!< default read time until refresh in milliseconds

		virtual ~socket() = default;

		/**
		 * \brief Get local address with local ip address and the tcp input port.
		 * \return local <i>inetaddress</i>
		 */
		[[nodiscard]] virtual const inetaddress& get_local_address() const noexcept = 0;

		/**
		 * \brief Get remote address with remote ip address and the tcp output stream.
		 * \return remote <i>inetaddress</i>
		 */
		[[nodiscard]] virtual const inetaddress& get_remote_address() const noexcept = 0;

		/**
		 * \brief Get the instance of the socket input stream, which refers to the socket file descriptor access.
		 * \return socket <i>inputstream</i>
		 */
		[[nodiscard]] virtual base::io::inputstream& get_inputstream() const noexcept = 0;

		/**
		 * \brief Get the instance of the socket output stream, which refers to the socket file descriptor access.
		 * \return socket <i>outputstream</i>
		 */
		[[nodiscard]] virtual base::io::outputstream& get_outputstream() const noexcept = 0;

		/**
		 * \brief Set the socket as interrupted.
		 *
		 * If set to true, any read call returns after the read blocking time expired and throws an <i>interruptedexception</i>.
		 * The flag must be cleared (set to false) by hand.
		 * @param interrupted if true, socket should get interrupted
		 */
		virtual void set_interrupted(bool interrupted) noexcept = 0;

		/**
		 * @returns true if socket should get interrupted
		 */
		[[nodiscard]] virtual bool get_interrupted() const noexcept = 0;

		/**
		 * \brief Set the socket timeout.
		 *
		 * If a call of read() on the <i>inputstream</i> takes longer than this timeout, an <i>timeoutexception</i> is thrown.
		 * \param timeout for reading
		 */
		virtual void set_timeout(timeout_t timeout) noexcept = 0;

		/**
		 * \brief Get the socket timeout.
		 * \return timeout
		 */
		[[nodiscard]] virtual timeout_t get_timeout() const noexcept = 0;

	};

} // namespace devfix::net
