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
  inetaddress inetaddress{};
  sockaddr_in sockaddr{};
  ASSERT_THROW(inetaddress.get_linux_family(), socketexception);
  ASSERT_THROW(inetaddress.set_to_sockaddr(sockaddr), socketexception);
  ASSERT_THROW(inetaddress.get_from_sockaddr(sockaddr), socketexception);

  inetaddress.family_ = inetaddress::family::IPV4;
  sockaddr.sin_family = inetaddress.get_linux_family();
  ASSERT_NO_THROW(inetaddress.get_linux_family());
  ASSERT_NO_THROW(inetaddress.set_to_sockaddr(sockaddr));
  ASSERT_NO_THROW(inetaddress.get_from_sockaddr(sockaddr));

}

TEST(Inetaddress, LinuxFamily)
{
    inetaddress inetaddress;
    ASSERT_EQ(inetaddress.family_, inetaddress::family::UNSUPPORTED);

    inetaddress.set_linux_family(AF_INET);
    ASSERT_EQ(inetaddress.family_, inetaddress::family::IPV4);

    inetaddress.family_ = inetaddress::family::IPV4;
    ASSERT_EQ(inetaddress.get_linux_family(), AF_INET);
}
#endif

TEST(Inetaddress, CreateByHost)
{
    inetaddress inetaddress = inetaddress::create_by_host("localhost", 80);
    ASSERT_EQ(inetaddress.port_, 80);
    ASSERT_EQ(inetaddress.family_, inetaddress::family::IPV4);
    ASSERT_EQ(inetaddress.address_.s_addr, 0x0100007F); // check for 127.0.0.1
    ASSERT_EQ(inetaddress.get_host(), "127.0.0.1");
}
