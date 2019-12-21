//
// Created by core on 12/19/19.
//



#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include "lnx_socket.h"
#include "../socketexception.h"
#include "../../base/timeoutexception.h"
#include "../../base/interruptedexception.h"

#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace devfix::net::lnx
{

lnx_socket::lnx_socket(const inetaddress &inetaddress) :
    remote_address_(inetaddress),
    local_address_()
{
  int family = (remote_address_.family_ == inetaddress::family::IPV4) ? AF_INET : 0;
  exception_guard_m(!family, socketexception, "unsupported address family");

  // create socket
  fd_ = ::socket(family, SOCK_STREAM, 0);
  exception_guard_m(fd_ < 0, socketexception, "create failed");

  // connect socket to remote address
  struct sockaddr_in sockaddr_remote{};
  remote_address_.set_sockaddr(sockaddr_remote);
  int err = ::connect(fd_, reinterpret_cast<struct sockaddr *>(&sockaddr_remote), sizeof(sockaddr_remote));
  exception_guard_m(err, socketexception, "connect failed");

  // get local address
  struct sockaddr_in sockaddr_local{};
  ::socklen_t sockaddr_local_length = sizeof(sockaddr_local);
  err = ::getsockname(fd_, reinterpret_cast<sockaddr *>(&sockaddr_local), &sockaddr_local_length);
  exception_guard_m(err, socketexception, "getsockname failed");
  local_address_.get_sockaddr(sockaddr_local);

  // set socket timeout to enable non blocking mode
  _set_read_blocking_time();
}

lnx_socket::~lnx_socket()
{
  if (!closed())
  {
    close();
  }
}

inetaddress lnx_socket::get_local_address() const noexcept
{
  return local_address_;
}

inetaddress lnx_socket::get_remote_address() const noexcept
{
  return remote_address_;
}

void lnx_socket::read(char *buf, std::size_t len)
{
  timeout_t time = 0;
  while (len)
  {
    if (interrupted())
    {
      throw base::interruptedexception(SOURCE_LINE);
    }

    if (time > timeout_)
    {
      throw base::timeoutexception(SOURCE_LINE);
    }

    std::size_t n = std::min(out_buf_.size(), len);
    ssize_t rc = ::read(fd_, buf, n);
    if (rc < 0)
    {
      if (errno == EAGAIN)
      {
        time += DEFAULT_READ_BLOCKING_TIME;
      } else
      {
        exception_guard(rc, socketexception);
      }
    } else if (rc > 0)
    {
      len -= static_cast<std::size_t>(rc);
      buf += rc;
    }
  }
}

void lnx_socket::write(const char *buf, std::size_t len)
{
  while (len)
  {
    std::size_t n = std::min(get_out_buf_size() - out_buf_idx_, len);
    std::memcpy(out_buf_.data() + out_buf_idx_, buf, n);
    out_buf_idx_ += n;
    buf += n;
    len -= n;
    if (out_buf_idx_ == get_out_buf_size())
    {
      flush();
    }
  }
}

void lnx_socket::flush()
{
  if (out_buf_idx_)
  {
    ssize_t err = ::write(fd_, out_buf_.data(), out_buf_idx_);
    out_buf_idx_ = 0;
    exception_guard(err, socketexception);
  }
}

std::size_t lnx_socket::available() const
{
  return _queue_available() + in_buf_idx_;
}

void lnx_socket::close()
{
  int err = ::close(fd_);
  fd_ = -1;
  exception_guard(err, socketexception);
}

bool lnx_socket::closed() const
{
  return fd_ < 0;
}

void lnx_socket::set_interrupted(bool interrupted) noexcept
{
  interrupted_ = interrupted;
}

bool lnx_socket::interrupted() const noexcept
{
  return interrupted_;
}

void lnx_socket::set_timeout(devfix::net::socket::timeout_t timeout) noexcept
{
  timeout_ = timeout;
}

socket::timeout_t lnx_socket::get_timeout() const noexcept
{
  return timeout_;
}

void lnx_socket::set_out_buf_size(std::size_t size) noexcept
{
  out_buf_.resize(size);
}

std::size_t lnx_socket::get_out_buf_size() const noexcept
{
  return out_buf_.size();
}

std::size_t lnx_socket::get_out_buf_available() const noexcept
{
  return out_buf_idx_;
}

void lnx_socket::set_in_buf_size(std::size_t size) noexcept
{
  in_buf_.resize(size);
}

std::size_t lnx_socket::get_in_buf_size() const noexcept
{
  return in_buf_.size();
}

std::size_t lnx_socket::get_in_buf_available() const noexcept
{
  return in_buf_idx_;
}

void lnx_socket::_set_read_blocking_time()
{
  // https://stackoverflow.com/a/2939145/10574851
  struct timeval tv{
      DEFAULT_READ_BLOCKING_TIME / 1000,
      (DEFAULT_READ_BLOCKING_TIME % 1000) * 1000
  };
  int err = ::setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&tv), sizeof tv);
  exception_guard(err, socketexception);
}

std::size_t lnx_socket::_queue_available() const
{
  int n;
  exception_guard(ioctl(fd_, FIONREAD, &n), socketexception);
  return static_cast<std::size_t>(n);
}

} // namespace devfix::net::lnx

#endif
