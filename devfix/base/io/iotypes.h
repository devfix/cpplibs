//
// Created by core on 12/27/19.
//

#pragma once

#include <functional>

namespace devfix::base::io
{

// general
typedef std::function<void()> close_t;
typedef std::function<bool()> is_closed_t;
const close_t DEFAULT_CLOSE = []()
{};
const is_closed_t DEFAULT_IS_CLOSED = []()
{ return false; };

// inputstream
typedef std::function<void(void *, std::size_t)> read_t;
typedef std::function<void(std::size_t)> skip_t;
typedef std::function<std::size_t()> available_t;

// outputstream
typedef std::function<void(void *, std::size_t)> write_t;
typedef std::function<void()> flush_t;

} // namespace devfix::base::io
