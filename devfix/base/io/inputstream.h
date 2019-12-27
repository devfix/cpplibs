//
// Created by core on 12/27/19.
//

#pragma once

#include <cstdint>

namespace devfix::base::io
{

struct inputstream
{
  virtual ~inputstream() = default;

  virtual void read(void *buf, std::size_t len) = 0;

  virtual void skip(std::size_t n) = 0;

  /**
   * @returns the number of bytes that are immediately available for reading
   */
  [[nodiscard]] virtual std::size_t available() = 0;

  /**
   * \brief Closes this socket.
   *  Once a socket has been closed, it is not available for further networking use
   *  (i.e. can't be reconnected or rebound).
   *  A new socket needs to be created.
   */
  virtual void close() = 0;

  /**
   * @returns true if the close() function got previously called
   */
  [[nodiscard]] virtual bool is_closed() = 0;

};

} // namespace devfix::base::io