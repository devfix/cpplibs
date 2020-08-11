//
// Created by core on 12/29/19.
//

#if CPPLIBS_ENABLE_NET_TESTS == 1

#include <catch2/catch.hpp>
#include "../inetaddress.h"
#include "../socketexception.h"

using namespace devfix::net;

#if PLATFORM_LINUX == 1
TEST_CASE("Inetaddress - BadFamily")
{
	inetaddress inetaddr{};
	sockaddr_in sockaddr{};

	REQUIRE_THROWS(NOT_USED(inetaddr.get_linux_family()));
	REQUIRE_THROWS(inetaddr.operator sockaddr_in());
	REQUIRE_THROWS(inetaddress(sockaddr));

	inetaddr = inetaddress("127.0.0.1", 0, inetaddress::family_t::IPV4);
	sockaddr.sin_family = inetaddr.get_linux_family();
	REQUIRE_NOTHROW(NOT_USED(inetaddr.get_linux_family()));
	REQUIRE_NOTHROW(inetaddr.operator sockaddr_in());
	REQUIRE_NOTHROW(inetaddress(sockaddr));
}

TEST_CASE("Inetaddress - LinuxFamily")
{
	inetaddress inetaddr;
	REQUIRE(inetaddr.get_family() == inetaddress::family_t::UNSPECIFIED);

	inetaddr.set_linux_family(AF_INET);
	REQUIRE(inetaddr.get_family()== inetaddress::family_t::IPV4);

	inetaddr = inetaddress("127.0.0.1", 0, inetaddress::family_t::IPV4);
	REQUIRE(inetaddr.get_linux_family()== AF_INET);
}
#endif

TEST_CASE("Inetaddress - CreateByHost")
{
	inetaddress inetaddr("localhost", 80);
	REQUIRE(inetaddr.get_port() == 80);
	REQUIRE(inetaddr.get_family() ==inetaddress::family_t::IPV4);
	REQUIRE(inetaddr.get_address() == 0x0100007F); // check for 127.0.0.1
	REQUIRE(inetaddr.get_host() == "127.0.0.1");
}

#endif
