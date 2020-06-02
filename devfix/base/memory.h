//
// Created by core on 12/21/19.
//

#pragma once

#include <memory>

namespace devfix::base
{

	/**
	 * \brief Alias for std::unique_ptr.
	 */
	template<class T>
	using up=std::unique_ptr<T>;

	/**
	 * \brief Alias for std::shared_ptr.
	 */
	template<class T>
	using sp=std::shared_ptr<T>;

} // namespace devfix::base
