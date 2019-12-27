//
// Created by core on 12/19/19.
//


#pragma once

#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include <atomic>
#include "../socket.h"
#include "../inetaddress.h"
#include "../../base/io/source.h"
#include "../../base/io/sink.h"

namespace devfix::net::lnx
{

struct lnx_socket : public socket
{
  explicit lnx_socket(const inetaddress &inetaddress);
  ~lnx_socket() override;

  [[nodiscard]] inetaddress get_local_address() const noexcept override;
  [[nodiscard]] inetaddress get_remote_address() const noexcept override;

  [[nodiscard]] base::io::inputstream &get_inputstream() const noexcept override;
  [[nodiscard]] base::io::outputstream &get_outputstream() const noexcept override;

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
  base::up<base::io::source> source_;
  base::up<base::io::sink> sink_;

  void _set_read_blocking_time();
  void _read(void *buf, std::size_t len);
  void _write(void *buf, std::size_t len);
  void _skip(std::size_t n);
  void _flush();
  std::size_t _available();
  void _close();
  bool _is_closed();
};

} // namespace devfix::net::lnx

#endif
