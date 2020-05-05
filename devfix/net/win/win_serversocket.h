//
// Created by Praktikum on 05.05.2020.
//

#pragma once

#include "../../base/platform.h"

#if PLATFORM_WINDOWS == 1

#include "../serversocket.h"


// forward declaration for external builder
namespace devfix::net
{
struct [[maybe_unused]] netbuilder;
}

namespace devfix::net::win
{

struct win_serversocket : public serversocket
{
  friend struct net::netbuilder;

  ~win_serversocket() override;

  [[nodiscard]] std::unique_ptr<socket> accept() override;

  [[nodiscard]] const inetaddress &get_address() const noexcept override;

  [[nodiscard]] bool get_reuse_address() const noexcept override;

  void set_accept_timeout(socket::timeout_t timeout) override;
  [[nodiscard]] socket::timeout_t get_accept_timeout() const noexcept override;

  void close() override;
  [[nodiscard]] bool is_closed() const noexcept override;
};

} // namespace devfix::net::win

#endif
