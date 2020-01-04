#pragma once

#include "../base/exception/baseexception.h"

namespace devfix::net
{

/**
 * \brief Thrown to indicate that there is an error creating or accessing a Socket.
 */
struct socketexception : public base::baseexception
{

  /**
   * Constructs the error object with what_arg as explanatory string that can be accessed through what().
   * @param what_arg explanatory std::string
   * @param err c error code (errno)
   */
  explicit socketexception(const std::string &what_arg, int err = -1) : base::baseexception(what_arg, err)
  {}

  /**
   * Constructs the error object with what_arg as explanatory string that can be accessed through what().
   * @param what_arg explanatory c-string
   * @param err c error code (errno)
   */
  explicit socketexception(const char *what_arg, int err = -1) : base::baseexception(what_arg, err)
  {}
};

} // namespace devfix::net
