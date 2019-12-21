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

void inetaddress::get_sockaddr(struct sockaddr_in &sockaddr)
{
  family_ = sockaddr.sin_family == AF_INET ? family::IPV4 : family::UNSUPPORTED;
  port_ = ntohs(sockaddr.sin_port);
  address_.s_addr = sockaddr.sin_addr.s_addr;
  exception_guard_m(family_ == family::UNSUPPORTED, socketexception, "unsupported address family");
}

void inetaddress::set_sockaddr(struct sockaddr_in &sockaddr) const
{
  ::bzero(&sockaddr, sizeof(sockaddr));
  sockaddr.sin_family = (family_ == family::IPV4) ? AF_INET : 0;
  sockaddr.sin_port = htons(port_);
  sockaddr.sin_addr.s_addr = address_.s_addr;
  exception_guard_m(!sockaddr.sin_family, socketexception, "unsupported address family");
}

#endif

} // namespace devfix::net
