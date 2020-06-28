//
// Created by core on 12/21/19.
//


#pragma once

#include "socket.h"

#if PLATFORM_LINUX == 1
#include "lnx/socket.h"
#include "lnx/serversocket.h"
#elif PLATFORM_WINDOWS == 1
#include "win/win_socket.h"
#include "win/win_serversocket.h"
#else
PLATPLATFORM_UNSUPPORTED;
#endif


namespace devfix::net
{

/**
 * \brief Builder class for platform independent instantiation.
 *
 * This class is the only one which has access to the constructors of network classes.
 */
	struct netbuilder
	{
		/**
		 * \brief Allow no instances of builder class
		 */
		netbuilder() = delete;

		/**
		 * \brief Creates a socket and connects it to the specified remote internet address.
		 * @param adr remote address
		 * @return connected socket with platform specific implementation
		 */
		static std::unique_ptr<socket> create_socket(inetaddress adr);

		/**
		 * \brief Creates a server socket and binds it to the supplied local address.
		 * \param adr local address for access restriction and port to listen on
		 * \param reuse_address if true allow bind to a port which remains in TIME_WAIT state
		 * \return socket in listen state with platform specific implementation
		 */
		static std::unique_ptr<serversocket> create_serversocket(inetaddress adr, bool reuse_address = false);

	};

} // namespace devfix::net
