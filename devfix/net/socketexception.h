#pragma once

#include "../base/exception.h"

namespace devfix::net
{

/**
 * Exception for server problems, for instance buffer overflow or i/o-errors.
 * @tparam T Type of character
 */

struct socketexception : public util::exception
{

  /**
   * Constructs the exception object with what_arg as explanatory std::string that can be accessed through what().
   * @param what_arg description
   */
  explicit socketexception(const std::string &what_arg, int err = -1) : util::exception(what_arg, err)
  {}

  /**
   * Constructs the exception object with what_arg as explanatory c-string that can be accessed through what().
   * @param what_arg description
   */
  explicit socketexception(const char *what_arg, int err = -1) : util::exception(what_arg, err)
  {}
};

} // namespace devfix::net
