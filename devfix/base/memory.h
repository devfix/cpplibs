//
// Created by core on 12/21/19.
//

#pragma once

#include <memory>

namespace devfix::base
{

template<class T>
using up=std::unique_ptr<T>;

template<class T>
using sp=std::shared_ptr<T>;

} // namespace devfix::base
