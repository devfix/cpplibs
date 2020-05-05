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
#endif

namespace devfix::net {

inetaddress::inetaddress(const std::string &host, port_t port, family_t family) :
    port_(port),
    family_(family) {

#if PLATFORM_LINUX == 1
  // hostent may points to static data, no free() required
  struct hostent *hostent = ::gethostbyname(host.data());
  exception_guard(!hostent, socketexception);
  address_ = *reinterpret_cast<address_t *>(hostent->h_addr);
#endif

}

#if PLATFORM_LINUX == 1

inetaddress::inetaddress(const struct sockaddr_in &sockaddr) {
  set_linux_family(sockaddr.sin_family);
  port_ = ntohs(sockaddr.sin_port);
  address_ = sockaddr.sin_addr.s_addr;
}

inetaddress::operator sockaddr_in() const {
  sockaddr_in sockaddr{};
  ::bzero(&sockaddr, sizeof(sockaddr));
  sockaddr.sin_family = get_linux_family();
  sockaddr.sin_port = htons(port_);
  sockaddr.sin_addr.s_addr = address_;
  return sockaddr;
}

sa_family_t inetaddress::get_linux_family() const {
  sa_family_t address_family = (family_ == inetaddress::family_t::IPV4) ? AF_INET : 0;
  exception_guard_m(!address_family, socketexception, "unsupported address family_t");
  return address_family;
}

void inetaddress::set_linux_family(sa_family_t address_family) {
  family_ = (address_family == AF_INET) ? family_t::IPV4 : family_t::UNSUPPORTED;
  exception_guard_m(family_ == family_t::UNSUPPORTED, socketexception, "unsupported address family_t");
}

#endif

std::string inetaddress::get_host() const noexcept {

#if PLATFORM_LINUX == 1
  return inet_ntoa(in_addr{address_});
#else
  #pragma message ("Platform not supported")
  return "";
#endif
}

inetaddress::address_t inetaddress::get_address() const {
  return address_;
}

inetaddress::port_t inetaddress::get_port() const {
  return port_;
}

inetaddress::family_t inetaddress::get_family() const {
  return family_;
}

} // namespace devfix::net
