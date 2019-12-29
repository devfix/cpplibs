//
// Created by core on 12/21/19.
//


#pragma once

#include "test_socket.h"

#if PLATFORM_LINUX == 1
#include "lnx/lnx_socket.h"
#include "lnx/lnx_serversocket.h"
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
  static std::unique_ptr<socket> create_socket(inetaddress adr);

  static std::unique_ptr<serversocket> create_serversocket(inetaddress adr, bool reuse_address = false);

};

} // namespace devfix::net
