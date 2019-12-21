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
  family family_;
  port_t port_;
  address address_;

  [[nodiscard]] static inetaddress create_by_host(family family, port_t port, const std::string &host);

  [[nodiscard]] std::string get_host() const noexcept;

#if PLATFORM_LINUX == 1
  void get_sockaddr(struct sockaddr_in &sockaddr);
  void set_sockaddr(struct sockaddr_in &sockaddr) const;
#endif

} __attribute__((packed));

} // namespace devfix::net
