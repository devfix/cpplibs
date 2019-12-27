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
#include <linux/tcp.h>
#include <netinet/in.h>

namespace devfix::net::lnx
{

lnx_socket::lnx_socket(const inetaddress &inetaddress) :
    remote_address_(inetaddress),
    local_address_(),
    source_(std::make_unique<base::io::source>(
        std::bind(&lnx_socket::_read, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&lnx_socket::_skip, this, std::placeholders::_1),
        std::bind(&lnx_socket::_available, this),
        std::bind(&lnx_socket::_close, this),
        std::bind(&lnx_socket::_is_closed, this)
    )),
    sink_(std::make_unique<base::io::sink>(
        std::bind(&lnx_socket::_write, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&lnx_socket::_flush, this),
        std::bind(&lnx_socket::_close, this),
        std::bind(&lnx_socket::_is_closed, this)
    ))
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
  if (!_is_closed())
  {
    _close();
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

base::io::inputstream &lnx_socket::get_inputstream() const noexcept
{
  return *source_;
}

base::io::outputstream &lnx_socket::get_outputstream() const noexcept
{
  return *sink_;
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

void lnx_socket::_read(void *buf, std::size_t len)
{
  char *c_ptr = static_cast<char *>(buf);
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

    ssize_t rc = ::read(fd_, c_ptr, len);
    if (rc < 0)
    {
      if (errno == EAGAIN)
      {
        time += DEFAULT_READ_BLOCKING_TIME;
      } else
      {
        exception_guard(rc, socketexception);
      }
    } else
    {
      len -= static_cast<std::size_t>(rc);
      c_ptr += rc;
    }
  }
}

void lnx_socket::_write(void *buf, std::size_t len)
{
  char *c_ptr = static_cast<char *>(buf);
  while (len)
  {
    ssize_t rc = ::write(fd_, c_ptr, len);
    exception_guard(rc < 0, socketexception);
    len -= static_cast<std::size_t>(rc);
    c_ptr += rc;
  }
}

void lnx_socket::_skip(std::size_t n)
{
  exception_guard(::lseek(fd_, static_cast<long>(n), SEEK_CUR), socketexception);
}

void lnx::lnx_socket::_flush()
{
  // disable Nagle algorithm and re-enable it
  int flag = 1;
  int err = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
  exception_guard(err, socketexception);

  flag = 0;
  err = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
  exception_guard(err, socketexception);
}

std::size_t lnx_socket::_available()
{
  int n;
  exception_guard(::ioctl(fd_, FIONREAD, &n), socketexception);
  return static_cast<std::size_t>(n);
}

void lnx_socket::_close()
{
  int err = ::close(fd_);
  fd_ = -1;
  exception_guard(err, socketexception);
}

bool lnx_socket::_is_closed()
{
  return fd_ < 0;
}

} // namespace devfix::net::lnx

#endif
