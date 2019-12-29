//
// Created by core on 12/20/19.
//

#pragma once

#include "socket.h"

namespace devfix::net
{

struct serversocket
{
  virtual ~serversocket() = default;

  [[nodiscard]] virtual std::unique_ptr<socket> accept() = 0;

  [[nodiscard]] virtual const inetaddress &get_address() const noexcept = 0;
  [[nodiscard]] virtual bool get_reuse_address() const noexcept = 0;

  virtual void set_accept_timeout(socket::timeout_t timeout) = 0;
  [[nodiscard]] virtual socket::timeout_t get_accept_timeout() const noexcept = 0;

  virtual void close() = 0;
  [[nodiscard]] virtual bool is_closed() const noexcept = 0;
};

} // namespace devfix::net
