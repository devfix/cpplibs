//
// Created by Praktikum on 05.05.2020.
//

#pragma once

#include "../../base/platform.h"

#if PLATFORM_WINDOWS == 0

#include "../serversocket.h"


// forward declaration for external builder
namespace devfix::net
{
struct [[maybe_unused]] netbuilder;
}

namespace devfix::net::win
{

struct serversocket : public devfix::net::serversocket
{
  friend struct net::netbuilder;

  ~serversocket() override;

  [[nodiscard]] std::unique_ptr<devfix::net::socket> accept() override;

  [[nodiscard]] const inetaddress &get_address() const noexcept override;

  [[nodiscard]] bool get_reuse_address() const noexcept override;

  void set_accept_timeout(devfix::net::socket::timeout_t timeout) override;
  [[nodiscard]] devfix::net::socket::timeout_t get_accept_timeout() const noexcept override;

  void close() override;
  [[nodiscard]] bool is_closed() const noexcept override;
};

} // namespace devfix::net::win

#endif
