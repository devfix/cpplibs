//
// Created by core on 12/19/19.
//



#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include "lnx_socket.h"
#include "../socketexception.h"
#include "../../base/error/timeoutexception.h"
#include "../../base/error/interruptedexception.h"

#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/tcp.h>
#include <netinet/in.h>


namespace devfix::net::lnx
{

	lnx_socket::~lnx_socket()
	{
		_close();
	}

	const inetaddress& lnx_socket::get_local_address() const noexcept
	{
		return local_address_;
	}

	const inetaddress& lnx_socket::get_remote_address() const noexcept
	{
		return remote_address_;
	}

	base::io::inputstream& lnx_socket::get_inputstream() const noexcept
	{
		return *source_;
	}

	base::io::outputstream& lnx_socket::get_outputstream() const noexcept
	{
		return *sink_;
	}

	void lnx_socket::set_interrupted(bool interrupted) noexcept
	{
		interrupted_ = interrupted;
	}

	bool lnx_socket::get_interrupted() const noexcept
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

	lnx_socket::lnx_socket(inetaddress remote_address)
		:
		remote_address_(remote_address),
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
		// create socket
		fd_ = ::socket(remote_address_.get_linux_family(), SOCK_STREAM, 0);
		exception_guard(fd_ < 0, socketexception);

		// connect socket to remote address
		auto sockaddr_remote = static_cast<struct sockaddr_in>(remote_address);
		int rc = ::connect(fd_, reinterpret_cast<struct sockaddr*>(&sockaddr_remote), sizeof(sockaddr_remote));
		exception_guard(rc, socketexception);

		local_address_ = _get_local_address();
		inetaddress a;
		inetaddress b(a);

		// set socket timeouts to implement non blocking mode
		_configure_io_timeout(SO_RCVTIMEO, DEFAULT_READ_BLOCKING_TIME);
		_configure_io_timeout(SO_SNDTIMEO, DEFAULT_WRITE_BLOCKING_TIME);
	}

	lnx_socket::lnx_socket(int fd, devfix::net::inetaddress remote_address)
		:
		fd_(fd),
		remote_address_(remote_address),
		local_address_(_get_local_address()),
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
		// set socket timeouts to implement non blocking mode
		_configure_io_timeout(SO_RCVTIMEO, DEFAULT_READ_BLOCKING_TIME);
		_configure_io_timeout(SO_SNDTIMEO, DEFAULT_WRITE_BLOCKING_TIME);
	}

	inetaddress lnx_socket::_get_local_address() const
	{
		int rc;
		struct sockaddr_in sockaddr_in{};
		socklen_t socklen = sizeof(sockaddr_in);
		rc = getsockname(fd_, reinterpret_cast<struct sockaddr*>(&sockaddr_in), &socklen);
		exception_guard(rc, socketexception);
		return inetaddress(sockaddr_in);
	}

	void lnx_socket::_configure_io_timeout(int optname, timeout_t timeout)
	{
		// https://stackoverflow.com/a/2939145/10574851
		struct timeval tv{
			timeout / 1000,
			(timeout % 1000) * 1000
		};
		int rc = ::setsockopt(fd_, SOL_SOCKET, optname, reinterpret_cast<char*>(&tv), sizeof tv);
		exception_guard(rc, socketexception);
	}

	void lnx_socket::_read(void* buf, std::size_t len)
	{
		timeout_t time = 0;
		while (len)
		{
			if (get_interrupted())
			{
				throw base::error::interruptedexception(SOURCE_LINE);
			}

			if (time > timeout_)
			{
				throw base::error::timeoutexception(SOURCE_LINE);
			}

			ssize_t rc = ::read(fd_, buf, len);
			if (rc < 0)
			{
				if (errno == EAGAIN)
				{
					time += DEFAULT_READ_BLOCKING_TIME;
				}
				else
				{
					exception_guard(true, socketexception);
				}
			}
			else
			{
				len -= static_cast<std::size_t>(rc);
				buf = static_cast<char*>(buf) + rc;
			}
		}
	}

	void lnx_socket::_write(const void* buf, std::size_t len)
	{
		timeout_t time = 0;
		while (len)
		{
			if (get_interrupted())
			{
				throw base::error::interruptedexception(SOURCE_LINE);
			}

			if (time > timeout_)
			{
				throw base::error::timeoutexception(SOURCE_LINE);
			}

			ssize_t rc = ::write(fd_, buf, len);
			if (rc < 0)
			{
				if (errno == EAGAIN)
				{
					time += DEFAULT_WRITE_BLOCKING_TIME;
				}
				else
				{
					exception_guard(true, socketexception);
				}
			}
			else
			{
				len -= static_cast<std::size_t>(rc);
				buf = static_cast<const char*>(buf) + rc;
			}
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
		int rc = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
		exception_guard(rc, socketexception);

		flag = 0;
		rc = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
		exception_guard(rc, socketexception);
	}

	std::size_t lnx_socket::_available()
	{
		int n;
		exception_guard(::ioctl(fd_, FIONREAD, &n), socketexception);
		return static_cast<std::size_t>(n);
	}

	void lnx_socket::_close()
	{
		if (_is_closed())
		{
			return;
		}

		int rc = ::close(fd_);
		fd_ = -1;
		exception_guard(rc, socketexception);
	}

	bool lnx_socket::_is_closed()
	{
		return fd_ < 0;
	}

} // namespace devfix::net::lnx

#endif
