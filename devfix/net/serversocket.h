//
// Created by core on 12/20/19.
//

#pragma once

#include "socket.h"

namespace devfix::net
{

	/**
	 * \brief This class implements server sockets.
	 *
	 * A server socket waits for requests to come in over the network.
	 * It performs some operation based on that request, and then possibly returns a result to the requester.
	 */
	struct serversocket
	{
		virtual ~serversocket() = default;

		/**
		 * \brief Listens for a connection to be made to this socket and accepts it.
		 *
		 * The method blocks until a connection is made.
		 * A new Socket s is created and returned.
		 * \return client socket
		 */
		[[nodiscard]] virtual std::unique_ptr<socket> accept() = 0;

		/**
		 * \brief Get the <i>inetaddress</i> the server is listening on.
		 *
		 * The ip address of the inetaddress can be used to restrict the access of clients to the server.
		 * \return bound inetaddress
		 */
		[[nodiscard]] virtual const inetaddress& get_address() const noexcept = 0;

		/**
		 * \brief Get if binding to a port which remains in TIME_WAIT state is allowed.
		 * \return true if allowed
		 */
		[[nodiscard]] virtual bool get_reuse_address() const noexcept = 0;

		/**
		 * \brief Set the accept timeout.
		 *
		 * If a call of accept() take longer than this timeout, an timeoutexception is thrown.
		 * A timeout of zero disables this feature.
		 * \param timeout
		 */
		virtual void set_accept_timeout(socket::timeout_t timeout) = 0;

		/**
		 * \brief Get the accept timeout.
		 *
		 * If the timeout ist zero the feature is disabled.
		 * \return timeout
		 */
		[[nodiscard]] virtual socket::timeout_t get_accept_timeout() const noexcept = 0;

		/**
		 * \brief Closes this <i>serversocket</i> and releases any system resources associated.
		 *
		 * The general contract of close is that it closes the serversocket.
		 * A closed <i>serversocket</i> cannot perform io operations and cannot be reopened.
		 */
		virtual void close() = 0;

		/**
		 * \brief Returns if the <i>serversocket</i> is closed or available for further calls of io operations like accept().
		 *
		 * @return true if the <i>serversocket</i> got previously closed.
		 */
		[[nodiscard]] virtual bool is_closed() const noexcept = 0;
	};

} // namespace devfix::net
