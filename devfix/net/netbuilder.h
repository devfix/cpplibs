//
// Created by core on 12/21/19.
//


#pragma once

#include "socket.h"

#if PLATFORM_LINUX == 1
#include "linux/lnx_socket.h"
#else
PLATFORM_UNSUPPORTED;
#endif

namespace devfix::net
{

struct netbuilder
{
  /**
   * \brief Creates a socket and connects it to the specified remote internet address.
   * The Socket will also bind() to the local address and port supplied.
   * @param inetaddress remote address
   * @return
   */
  static base::up<socket> create_socket(inetaddress adr);

};

} // namespace devfix::net
