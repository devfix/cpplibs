//
// Created by core on 12/21/19.
//

#include "netbuilder.h"


namespace devfix::net
{

std::unique_ptr<socket> netbuilder::create_socket(inetaddress adr)
{
#if PLATFORM_LINUX == 1
  return std::unique_ptr<lnx::lnx_socket>(new lnx::lnx_socket(adr));
#endif
}

std::unique_ptr<serversocket> netbuilder::create_serversocket(devfix::net::inetaddress adr, bool reuse_address)
{
#if PLATFORM_LINUX == 1
  return std::unique_ptr<lnx::lnx_serversocket>(new lnx::lnx_serversocket(adr, reuse_address));
#endif
}

} // namespace devfix::net
