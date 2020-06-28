//
// Created by core on 12/19/19.
//



#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include "socket.h"
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

	socket::~socket() { _close(); }

	const inetaddress& socket::get_local_address() const noexcept { return local_address_; }

	const inetaddress& socket::get_remote_address() const noexcept { return remote_address_; }

	base::io::inputstream& socket::get_inputstream() const noexcept { return *source_; }

	base::io::outputstream& socket::get_outputstream() const noexcept { return *sink_; }

	void socket::set_interrupted(bool interrupted) noexcept { interrupted_ = interrupted; }

	bool socket::get_interrupted() const noexcept { return interrupted_; }

	void socket::set_timeout(devfix::net::socket::timeout_t timeout) noexcept { timeout_ = timeout; }

	socket::timeout_t socket::get_timeout() const noexcept { return timeout_; }

	socket::socket(inetaddress remote_address)
		:
		remote_address_(remote_address),
		local_address_(),
		source_(std::make_unique<base::io::source>(
			[&](void* buf, std::size_t len) { return _read(buf, len); },
			[&](std::size_t n) { return _skip(n); },
			[&] { return _available(); },
			[&] { return _close(); },
			[&] { return _is_closed(); })),
		sink_(std::make_unique<base::io::sink>(
			[&](const void* buf, std::size_t len) { _write(buf, len); },
			[&] { return _flush(); },
			[&] { return _close(); },
			[&] { return _is_closed(); }))
	{
		// create socket
		fd_ = ::socket(remote_address_.get_linux_family(), SOCK_STREAM, 0);
		EXCEPTION_GUARD_ERRNO(fd_ < 0, socketexception);

		// connect socket to remote address
		auto sockaddr_remote = static_cast<struct sockaddr_in>(remote_address);
		int rc = ::connect(fd_, reinterpret_cast<struct sockaddr*>(&sockaddr_remote), sizeof(sockaddr_remote));
		EXCEPTION_GUARD_ERRNO(rc, socketexception);

		local_address_ = _get_local_address();
		inetaddress a;
		inetaddress b(a);

		// set socket timeouts to implement non blocking mode
		_configure_io_timeout(SO_RCVTIMEO, DEFAULT_READ_BLOCKING_TIME);
		_configure_io_timeout(SO_SNDTIMEO, DEFAULT_WRITE_BLOCKING_TIME);
	}

	socket::socket(int fd, devfix::net::inetaddress remote_address)
		:
		fd_(fd),
		remote_address_(remote_address),
		local_address_(_get_local_address()),
		source_(std::make_unique<base::io::source>(
			[&](void* buf, std::size_t len) { return _read(buf, len); },
			[&](std::size_t n) { return _skip(n); },
			[&] { return _available(); },
			[&] { return _close(); },
			[&] { return _is_closed(); })),
		sink_(std::make_unique<base::io::sink>(
			[&](const void* buf, std::size_t len) { _write(buf, len); },
			[&] { return _flush(); },
			[&] { return _close(); },
			[&] { return _is_closed(); }))
	{
		// set socket timeouts to implement non blocking mode
		_configure_io_timeout(SO_RCVTIMEO, DEFAULT_READ_BLOCKING_TIME);
		_configure_io_timeout(SO_SNDTIMEO, DEFAULT_WRITE_BLOCKING_TIME);
	}

	inetaddress socket::_get_local_address() const
	{
		int rc;
		struct sockaddr_in sockaddr_in{};
		socklen_t socklen = sizeof(sockaddr_in);
		rc = getsockname(fd_, reinterpret_cast<struct sockaddr*>(&sockaddr_in), &socklen);
		EXCEPTION_GUARD_ERRNO(rc, socketexception);
		return inetaddress(sockaddr_in);
	}

	void socket::_configure_io_timeout(int optname, timeout_t timeout)
	{
		// https://stackoverflow.com/a/2939145/10574851
		struct timeval tv{
			timeout / 1000,
			(timeout % 1000) * 1000
		};
		int rc = ::setsockopt(fd_, SOL_SOCKET, optname, reinterpret_cast<char*>(&tv), sizeof tv);
		EXCEPTION_GUARD_ERRNO(rc, socketexception);
	}

	void socket::_read(void* buf, std::size_t len)
	{
		timeout_t time = 0;
		while (len)
		{
			EXCEPTION_GUARD (get_interrupted(), ::devfix::base::error::interruptedexception);
			EXCEPTION_GUARD (time > timeout_, ::devfix::base::error::timeoutexception);

			ssize_t rc = ::read(fd_, buf, len);
			if (rc > 0)
			{
				len -= static_cast<std::size_t>(rc);
				buf = static_cast<char*>(buf) + rc;
			}
			else if (rc == 0 || errno == EAGAIN) { time += DEFAULT_READ_BLOCKING_TIME; }
			else { EXCEPTION_GUARD_ERRNO(true, socketexception); }
		}
	}

	void socket::_write(const void* buf, std::size_t len)
	{
		timeout_t time = 0;
		while (len)
		{
			EXCEPTION_GUARD(get_interrupted(), ::devfix::base::error::interruptedexception);
			EXCEPTION_GUARD (time > timeout_, ::devfix::base::error::timeoutexception);

			ssize_t rc = ::write(fd_, buf, len);
			if (rc > 0)
			{
				len -= static_cast<std::size_t>(rc);
				buf = static_cast<const char*>(buf) + rc;
			}
			else if (rc == 0 || errno == EAGAIN) { time += DEFAULT_WRITE_BLOCKING_TIME; }
			else { EXCEPTION_GUARD_ERRNO(true, socketexception); }
		}
	}

	void socket::_skip(std::size_t n)
	{
		EXCEPTION_GUARD_ERRNO(::lseek(fd_, static_cast<long>(n), SEEK_CUR), socketexception);
	}

	void lnx::socket::_flush()
	{
		// disable Nagle algorithm and re-enable it
		int flag = 1;
		int rc = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
		EXCEPTION_GUARD_ERRNO(rc, socketexception);

		flag = 0;
		rc = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
		EXCEPTION_GUARD_ERRNO(rc, socketexception);
	}

	std::size_t socket::_available()
	{
		int n;
		EXCEPTION_GUARD_ERRNO(::ioctl(fd_, FIONREAD, &n), socketexception);
		return static_cast<std::size_t>(n);
	}

	void socket::_close()
	{
		if (_is_closed()) { return; }

		int rc = ::close(fd_);
		fd_ = -1;
		EXCEPTION_GUARD_ERRNO(rc, socketexception);
	}

	bool socket::_is_closed() { return fd_ < 0; }

} // namespace devfix::net::lnx

#endif
