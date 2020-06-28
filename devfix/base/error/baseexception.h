//
// Created by core on 12/19/19.
//

#pragma once

#include <exception>
#include <string>
#include <cstring>


#define EXCEPTION_GUARD_ERRNO(err, exception_class) \
  if (err) throw exception_class(std::string(std::strerror(errno)) + " @ " + SOURCE_LINE, errno)

#define EXCEPTION_GUARD_MSG(err, exception_class, message) \
  if (err) throw exception_class(std::string(message) + " @ " + SOURCE_LINE, errno)

#define EXCEPTION_GUARD(err, exception_class) \
  if (err) throw exception_class(std::string(message) + " @ " + SOURCE_LINE, 0)


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
		 * @param err platform error code (errno)
		 */
		explicit baseexception(const std::string& what_arg, int err = 0) : what_arg_(what_arg), err_(err) {}

		/**
		 * Virtual constructor to make class abstract.
		 */
		~baseexception() override = default;

		/**
		 * Returns a C-style character string describing the general cause of the current error.
		 * @return explanatory string
		 */
		[[nodiscard]] const char* what() const noexcept final { return what_arg_.data(); }

		/**
		 * \brief Returns a platform specific error code which corresponds with the exception cause.
		 * A value of '0' means unspecified error code.
		 * \return error code
		 */
		[[nodiscard]] int get_code() const noexcept { return err_; }

	protected:
		std::string what_arg_; //!< failure description
		int err_; //!< platform specific error code
	};

} // namespace devfix::base::error
