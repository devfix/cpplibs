//
// Created by core on 12/29/19.
//

#include <gtest/gtest.h>
#include "../inetaddress.h"
#include "../socketexception.h"

using namespace devfix::net;

#if PLATFORM_LINUX == 1
TEST(Inetaddress, BadFamily)
{
	inetaddress inetaddr{};
	sockaddr_in sockaddr{};

	ASSERT_THROW(inetaddr.get_linux_family(), socketexception);
	ASSERT_THROW(inetaddr.operator sockaddr_in(), socketexception);
	ASSERT_THROW(inetaddress ia(sockaddr), socketexception);

	inetaddr = inetaddress("127.0.0.1", 0, inetaddress::family_t::IPV4);
	sockaddr.sin_family = inetaddr.get_linux_family();
	ASSERT_NO_THROW(inetaddr.get_linux_family());
	ASSERT_NO_THROW(inetaddr.operator sockaddr_in());
	ASSERT_NO_THROW(inetaddress ia(sockaddr));
}

TEST(Inetaddress, LinuxFamily)
{
	inetaddress inetaddr;
	ASSERT_EQ(inetaddr.get_family(), inetaddress::family_t::UNSUPPORTED);

	inetaddr.set_linux_family(AF_INET);
	ASSERT_EQ(inetaddr.get_family(), inetaddress::family_t::IPV4);

	inetaddr = inetaddress("127.0.0.1", 0, inetaddress::family_t::IPV4);
	ASSERT_EQ(inetaddr.get_linux_family(), AF_INET);
}
#endif

TEST(Inetaddress, CreateByHost)
{
	inetaddress inetaddr("localhost", 80);
	ASSERT_EQ(inetaddr.get_port(), 80);
	ASSERT_EQ(inetaddr.get_family(), inetaddress::family_t::IPV4);
	ASSERT_EQ(inetaddr.get_address(), 0x0100007F); // check for 127.0.0.1
	ASSERT_EQ(inetaddr.get_host(), "127.0.0.1");

	throw std::runtime_error(SOURCE_LINE);
}
