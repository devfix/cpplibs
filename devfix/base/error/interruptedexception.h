#pragma once

#include "baseexception.h"

namespace devfix::base::error
{

/**
 * \brief Thrown when an operation is interrupted, either before or during the activity.
 *
 * Occasionally a method may wish to test whether the current operation has been interrupted, and if so, to immediately throw this error.
 */
	struct interruptedexception : public baseexception
	{

		/**
		 * Constructs the error object with what_arg as explanatory string that can be accessed through what().
		 * @param what_arg explanatory std::string
		 * @param err c error code (errno)
		 */
		explicit interruptedexception(const std::string& what_arg, int err = 0)
			: baseexception(what_arg, err)
		{
		}

		/**
		 * Constructs the error object with what_arg as explanatory string that can be accessed through what().
		 * @param what_arg explanatory c-string
		 * @param err c error code (errno)
		 */
		explicit interruptedexception(const char* what_arg, int err = 0)
			: baseexception(what_arg, err)
		{
		}
	};

} // namespace devfix::base::error
