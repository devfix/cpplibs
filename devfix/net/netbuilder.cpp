//
// Created by core on 12/21/19.
//

#include "netbuilder.h"


namespace devfix::net
{

base::up<socket> netbuilder::create_socket(inetaddress adr)
{

#if PLATFORM_LINUX == 1
  return std::make_unique<lnx::lnx_socket>(adr);
#endif

}

} // namespace devfix::net
