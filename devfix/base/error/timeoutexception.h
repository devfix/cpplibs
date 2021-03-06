#pragma once

#include "baseexception.h"

namespace devfix::base::error
{

/**
 * \brief Exception thrown when a blocking operation times out.
 *
 * Blocking operations for which a timeout is specified need a means to indicate that the timeout has occurred.
 * For many such operations it is possible to return a value that indicates timeout; when that is not possible or desirable then TimeoutException should be declared and thrown.
 */
	struct timeoutexception : public baseexception
	{

		/**
		 * Constructs the error object with what_arg as explanatory string that can be accessed through what().
		 * @param what_arg explanatory std::string
		 * @param err c error code (errno)
		 */
		explicit timeoutexception(const std::string& what_arg, int err = -1)
			: baseexception(what_arg, err)
		{
		}

		/**
		 * Constructs the error object with what_arg as explanatory string that can be accessed through what().
		 * @param what_arg explanatory c-string
		 * @param err c error code (errno)
		 */
		explicit timeoutexception(const char* what_arg, int err = -1)
			: baseexception(what_arg, err)
		{
		}
	};

} // namespace devfix::base::error
