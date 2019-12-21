#pragma once

#include "exception.h"

namespace devfix::base
{

/**
 * Exception for timeouts.
 * @tparam T Type of character
 */

struct timeoutexception : public base::exception
{

  /**
   * Constructs the exception object with what_arg as explanatory std::string that can be accessed through what().
   * @param what_arg description
   */
  explicit timeoutexception(const std::string &what_arg, int err = -1) : base::exception(what_arg, err)
  {}

  /**
   * Constructs the exception object with what_arg as explanatory c-string that can be accessed through what().
   * @param what_arg description
   */
  explicit timeoutexception(const char *what_arg, int err = -1) : base::exception(what_arg, err)
  {}
};

} // namespace devfix::base
