//
// Created by core on 12/18/19.
//


#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <atomic>
#include "inetaddress.h"
#include "../base/memory.h"
#include "../base/platform.h"


namespace devfix::net
{

struct socket
{
  typedef std::uint32_t timeout_t;
  static constexpr std::size_t DEFAULT_OUT_BUF_SIZE = 1024; //!< default buffer size in bytes
  static constexpr std::size_t DEFAULT_IN_BUF_SIZE = 1024; //!< default buffer size in bytes
  static constexpr timeout_t DEFAULT_TIMEOUT = 3000; //!< default read timeout in milliseconds
  static constexpr timeout_t DEFAULT_READ_BLOCKING_TIME = 100; //!< default read timeout until refresh in milliseconds

  virtual ~socket() = default;

  [[nodiscard]] virtual inetaddress get_local_address() const noexcept = 0;
  [[nodiscard]] virtual inetaddress get_remote_address() const noexcept = 0;

  virtual void read(char *buf, std::size_t len) = 0;
  virtual void write(const char *buf, std::size_t len) = 0;

  virtual void flush() = 0;

  /**
   * Get the number of bytes that are immediately available for	reading.
   * @return number of bytes
   */
  [[nodiscard]] virtual std::size_t available() const = 0;

  /**
   * \brief Closes this socket.
   *  Once a socket has been closed, it is not available for further networking use
   *  (i.e. can't be reconnected or rebound).
   *  A new socket needs to be created.
   */
  virtual void close() = 0;
  [[nodiscard]] virtual bool closed() const = 0;

  virtual void set_interrupted(bool interrupted) noexcept = 0;
  [[nodiscard]] virtual bool interrupted() const noexcept = 0;

  virtual void set_timeout(timeout_t timeout) noexcept = 0;
  [[nodiscard]] virtual timeout_t get_timeout() const noexcept = 0;

  virtual void set_out_buf_size(std::size_t size) noexcept = 0;
  [[nodiscard]] virtual std::size_t get_out_buf_size() const noexcept = 0;
  [[nodiscard]] virtual std::size_t get_out_buf_available() const noexcept = 0;

  virtual void set_in_buf_size(std::size_t size) noexcept = 0;
  [[nodiscard]] virtual std::size_t get_in_buf_size() const noexcept = 0;
  [[nodiscard]] virtual std::size_t get_in_buf_available() const noexcept = 0;

};

} // namespace devfix::net
