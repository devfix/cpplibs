//
// Created by core on 12/19/19.
//


#pragma once

#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include "../socket.h"
#include "../inetaddress.h"

namespace devfix::net::lnx
{

struct lnx_socket : public socket
{
  explicit lnx_socket(const inetaddress &inetaddress);
  ~lnx_socket() override;

  [[nodiscard]] inetaddress get_local_address() const noexcept override;
  [[nodiscard]] inetaddress get_remote_address() const noexcept override;

  void read(char *buf, std::size_t len) override;
  void write(const char *buf, std::size_t len) override;

  void flush() override;
  [[nodiscard]] std::size_t available() const override;

  void close() override;
  [[nodiscard]] bool closed() const override;

  void set_interrupted(bool interrupted) noexcept override;
  [[nodiscard]] bool interrupted() const noexcept override;

  void set_timeout(timeout_t timeout) noexcept override;
  [[nodiscard]] timeout_t get_timeout() const noexcept override;

 private:
  int fd_ = -1;
  inetaddress remote_address_;
  inetaddress local_address_;
  std::atomic_bool interrupted_ = false;
  timeout_t timeout_ = DEFAULT_TIMEOUT;

  void _set_read_blocking_time();
};

} // namespace devfix::net::lnx

#endif
