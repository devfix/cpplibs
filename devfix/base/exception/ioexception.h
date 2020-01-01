#pragma once

#include "exception.h"

namespace devfix::base
{

/**
 * \brief Signals that an I/O exception of some sort has occurred.
 *
 * This class is the general class of exceptions produced by failed or interrupted I/O operations.
 */
struct ioexception : public baseexception
{

  /**
   * Constructs the exception object with what_arg as explanatory string that can be accessed through what().
   * @param what_arg explanatory std::string
   * @param err c error code (errno)
   */
  explicit ioexception(const std::string &what_arg, int err = -1) : baseexception(what_arg, err)
  {}

  /**
   * Constructs the exception object with what_arg as explanatory string that can be accessed through what().
   * @param what_arg explanatory c-string
   * @param err c error code (errno)
   */
  explicit ioexception(const char *what_arg, int err = -1) : baseexception(what_arg, err)
  {}
};

} // namespace devfix::base
