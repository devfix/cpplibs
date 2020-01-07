//
// Created by core on 12/19/19.
//

#pragma once

#include <exception>
#include <string>
#include <cstring>

namespace devfix::base::error
{

/**
 * \brief Abstract error base class.
 *
 * This class is the parent of more specific exceptions and cannot be thrown directly.
 */
struct baseexception : public std::exception
{

  /**
   * Delete simple constructor, always enforce a failure description.
   */
  baseexception() = delete;

  /**
   * Constructs the error object with what_arg as explanatory std::string that can be accessed through what().
   * @param what_arg failure description
   * @param err c error code (errno)
   */
  explicit baseexception(std::string what_arg, int err = -1) : what_arg_(std::move(what_arg)), err_(err)
  {}

  /**
   * Virtual constructor to make class abstract.
   */
  ~baseexception() override = default;

  /**
   * Returns a C-style character string describing the general cause of the current error.
   * @return explanatory string
   */
  [[nodiscard]] const char *what() const noexcept final
  {
    return what_arg_.data();
  }

  [[nodiscard]] int get_errno() const noexcept
  {
    return err_;
  }

 protected:
  std::string what_arg_; //!< failure description
  int err_;
};

#define exception_guard_m(err, exception_class, message) \
  if (err) \
    throw exception_class(message + std::string(" @ ") + SOURCE_LINE, errno)

#define exception_guard(err, exception_class) \
  exception_guard_m(err, exception_class, std::strerror(errno))

} // namespace devfix::base::error
