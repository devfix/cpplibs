//
// Created by core on 12/29/19.
//

#include <gtest/gtest.h>
#include "../inetaddress.h"
#include "../socketexception.h"

using namespace devfix::net;

#if PLATFORM_LINUX == 1
TEST(TestInetaddress, BadFamily)
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
#endif