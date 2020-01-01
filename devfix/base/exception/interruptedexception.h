#pragma once

#include "exception.h"

namespace devfix::base
{

/**
 * \brief Thrown when an operation is interrupted, either before or during the activity.
 *
 * Occasionally a method may wish to test whether the current operation has been interrupted, and if so, to immediately throw this exception.
 */
struct interruptedexception : public baseexception
{

  /**
   * Constructs the exception object with what_arg as explanatory string that can be accessed through what().
   * @param what_arg explanatory std::string
   * @param err c error code (errno)
   */
  explicit interruptedexception(const std::string &what_arg, int err = -1) : baseexception(what_arg, err)
  {}

  /**
   * Constructs the exception object with what_arg as explanatory string that can be accessed through what().
   * @param what_arg explanatory c-string
   * @param err c error code (errno)
   */
  explicit interruptedexception(const char *what_arg, int err = -1) : baseexception(what_arg, err)
  {}
};

} // namespace devfix::base
