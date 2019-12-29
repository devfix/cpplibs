//
// Created by core on 12/18/19.
//

#include <cstring>
#include "../base/platform.h"
#include "inetaddress.h"
#include "socketexception.h"

// platform specific includes
#if PLATFORM_LINUX == 1
#include <netdb.h>
#include <arpa/inet.h>
#else
PLATFORM_UNSUPPORTED;
#endif

namespace devfix::net
{

inetaddress inetaddress::create_by_host(const std::string &host, port_t port, family family)
{

#if PLATFORM_LINUX == 1
  // hostent my points to static data, no free() required
  struct hostent *hostent = ::gethostbyname(host.data());
  if (!hostent)
  {
    throw socketexception("nullptr @ " + SOURCE_LINE);
  }

  return inetaddress
      {
          *reinterpret_cast<union address *>(hostent->h_addr),
          port,
          family
      };
#endif

}

std::string inetaddress::get_host() const noexcept
{

#if PLATFORM_LINUX == 1
  return inet_ntoa(in_addr{address_.s_addr});
#endif

}

#if PLATFORM_LINUX == 1

void inetaddress::get_from_sockaddr(const struct sockaddr_in &sockaddr)
{
  set_linux_family(sockaddr.sin_family);
  port_ = ntohs(sockaddr.sin_port);
  address_.s_addr = sockaddr.sin_addr.s_addr;
}

void inetaddress::set_to_sockaddr(struct sockaddr_in &sockaddr) const
{
  ::bzero(&sockaddr, sizeof(sockaddr));
  sockaddr.sin_family = get_linux_family();
  sockaddr.sin_port = htons(port_);
  sockaddr.sin_addr.s_addr = address_.s_addr;
}

sa_family_t inetaddress::get_linux_family() const
{
  sa_family_t address_family = (family_ == inetaddress::family::IPV4) ? AF_INET : 0;
  exception_guard_m(!address_family, socketexception, "unsupported address family");
  return address_family;
}

void inetaddress::set_linux_family(sa_family_t address_family)
{
  family_ = (address_family == AF_INET) ? family::IPV4 : family::UNSUPPORTED;
  exception_guard_m(family_ == family::UNSUPPORTED, socketexception, "unsupported address family");
}

#endif

} // namespace devfix::net
