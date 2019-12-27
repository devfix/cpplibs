//
// Created by core on 12/27/19.
//

#include "source.h"

namespace devfix::base::io
{

source::source(read_t read, skip_t skip, available_t available, close_t close, is_closed_t is_closed) :
    read_(std::move(read)),
    skip_(std::move(skip)),
    available_(std::move(available)),
    close_(std::move(close)),
    is_closed_(std::move(is_closed))
{}

void source::read(void *buf, std::size_t len)
{
  read_(buf, len);
}

void source::skip(std::size_t n)
{
  skip_(n);
}

std::size_t source::available()
{
  return available_();
}

void source::close()
{
  close_();
}

bool source::is_closed()
{
  return is_closed_();
}

} // namespace devfix::base::io
