//
// Created by core on 12/27/19.
//

#include "sink.h"

#include <utility>

namespace devfix::base::io
{

sink::sink(devfix::base::io::write_t write,
           devfix::base::io::flush_t flush,
           devfix::base::io::close_t close,
           devfix::base::io::is_closed_t is_closed) :
    write_(std::move(write)),
    flush_(std::move(flush)),
    close_(std::move(close)),
    is_closed_(std::move(is_closed))
{}

void sink::write(void *buf, std::size_t len)
{
  write_(buf, len);
}

void sink::flush()
{
  flush_();
}

void sink::close()
{
  close_();
}

bool sink::is_closed()
{
  return is_closed_();
}

} // namespace devfix::base::io
