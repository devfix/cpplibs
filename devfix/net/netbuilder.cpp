//
// Created by core on 12/21/19.
//

#include "netbuilder.h"


namespace devfix::net
{

	std::unique_ptr<socket> netbuilder::create_socket([[maybe_unused]] inetaddress adr)
	{
#if PLATFORM_LINUX == 1
		return std::unique_ptr<lnx::socket>(new lnx::socket(adr));
#else
#pragma message ("Platform not supported")
		return {};
#endif
	}

	std::unique_ptr<serversocket> netbuilder::create_serversocket([[maybe_unused]] devfix::net::inetaddress adr,
		[[maybe_unused]] bool reuse_address)
	{
#if PLATFORM_LINUX == 1
		return std::unique_ptr<lnx::serversocket>(new lnx::serversocket(adr, reuse_address));
#else
#pragma message ("Platform not supported")
		return {};
#endif

	}

} // namespace devfix::net
