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

  void set_out_buf_size(std::size_t size) noexcept override;
  [[nodiscard]] std::size_t get_out_buf_size() const noexcept override;
  [[nodiscard]] std::size_t get_out_buf_available() const noexcept override;

  void set_in_buf_size(std::size_t size) noexcept override;
  [[nodiscard]] std::size_t get_in_buf_size() const noexcept override;
  [[nodiscard]] std::size_t get_in_buf_available() const noexcept override;

 private:
  int fd_ = -1;
  inetaddress remote_address_;
  inetaddress local_address_;
  std::atomic_bool interrupted_ = false;
  timeout_t timeout_ = DEFAULT_TIMEOUT;
  std::vector<char> out_buf_ = std::vector<char>(DEFAULT_OUT_BUF_SIZE);
  std::size_t out_buf_idx_ = 0;
  std::vector<char> in_buf_ = std::vector<char>(DEFAULT_IN_BUF_SIZE);
  std::size_t in_buf_idx_ = 0;

  void _set_read_blocking_time();

  /**
   * ufftata
   * @return
   */
  [[nodiscard]] std::size_t _queue_available() const;
};

} // namespace devfix::net::lnx

#endif
