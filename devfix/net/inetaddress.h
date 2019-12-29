//
// Created by core on 12/18/19.
//


#pragma once

#include "../base/platform.h"

#include <string>

#if PLATFORM_LINUX == 1
#include <netinet/in.h>
#endif

namespace devfix::net
{

struct inetaddress
{

  enum class family : char
  {
    UNSUPPORTED = 0,
    IPV4 = 1
  };

  typedef std::uint16_t port_t;

  union address
  {
    std::uint32_t s_addr;
    char s_bytes[4];
  };

  // keep variables public
  address address_ = {0};
  port_t port_ = 0;
  family family_ = family::UNSUPPORTED;

  [[nodiscard]] static inetaddress create_by_host(const std::string &host, port_t port, family family = family::IPV4);

  [[nodiscard]] std::string get_host() const noexcept;

#if PLATFORM_LINUX == 1
  void get_from_sockaddr(const struct sockaddr_in &sockaddr);
  void set_to_sockaddr(struct sockaddr_in &sockaddr) const;
  sa_family_t get_linux_family() const;
  void set_linux_family(sa_family_t address_family);
#endif

} __attribute__((packed));

} // namespace devfix::net
