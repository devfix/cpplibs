//
// Created by core on 12/19/19.
//

#pragma once

#include <exception>
#include <string>
#include "platform.h"

namespace devfix::util
{

/*! Abstract exception base class. */

struct exception : public std::exception
{

  /**
   * Delete simple constructor, always enforce a failure description.
   */
  exception() = delete;

  /**
   * Constructs the exception object with what_arg as explanatory std::string that can be accessed through what().
   * @param what_arg failure description
   */
  explicit exception(std::string what_arg, int err = -1) : what_arg_(std::move(what_arg))
  {}

  /**
   * Virtual constructor to make class abstract.
   */
  ~exception() override = default;

  /**
   * Returns a C-style character string describing the general cause of the current error.
   * @return explanatory string
   */
  [[nodiscard]] const char *what() const noexcept final
  {
    return what_arg_.data();
  }

 protected:
  std::string what_arg_; //!< failure description
};

#define exception_guard_m(err, exception_class, message) \
  if (err) \
    throw exception_class(message + std::string(" @ ") + SOURCE_LINE, errno)

#define exception_guard(err, exception_class) exception_guard_m(err, exception_class, std::strerror(errno))

} // namespace devfix::util
