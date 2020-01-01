//
// Created by core on 12/27/19.
//

#pragma once

#include <cstdint>

namespace devfix::base::io
{

struct outputstream
{
  virtual ~outputstream() = default;

  virtual void write(const void * buf, std::size_t len) = 0;

  virtual void flush() = 0;

  virtual void close() = 0;

  [[nodiscard]] virtual bool is_closed() = 0;
};

} // namespace devfix::base::io