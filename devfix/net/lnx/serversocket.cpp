//
// Created by core on 12/29/19.
//

#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include <unistd.h>
#include "serversocket.h"
#include "../../base/error/baseexception.h"
#include "../socketexception.h"
#include "socket.h"

namespace devfix::net::lnx
{

	serversocket::~serversocket()
	{
		close();
	}

	std::unique_ptr<devfix::net::socket> serversocket::accept()
	{
		struct sockaddr_in sockaddr_in{};
		socklen_t socklen = sizeof(sockaddr_in);
		int fd = ::accept(fd_, reinterpret_cast<sockaddr*>(&sockaddr_in), &socklen);
		EXCEPTION_GUARD_ERRNO(fd < 0, socketexception);

		inetaddress remote_address(sockaddr_in);
		return std::unique_ptr<socket>(new socket(fd, remote_address));
	}

	const inetaddress& serversocket::get_address() const
	{
		return local_address_;
	}

	bool serversocket::get_reuse_address() const
	{
		return reuse_address_;
	}

	void serversocket::set_accept_timeout([[maybe_unused]] socket::timeout_t timeout)
	{
		EXCEPTION_GUARD_MSG(true, socketexception, "This serversocket implementation does currently not support accept() with timeout.");
	}

	socket::timeout_t serversocket::get_accept_timeout() const
	{
		EXCEPTION_GUARD_MSG(true, socketexception, "This serversocket implementation does currently not support accept() with timeout.");
	}

	void serversocket::close()
	{
		if (is_closed())
		{
			return;
		}

		int rc = ::close(fd_);
		fd_ = -1;
		EXCEPTION_GUARD_ERRNO(rc, socketexception);
	}

	bool serversocket::is_closed() const
	{
		return fd_ < 0;
	}

	serversocket::serversocket(inetaddress inetaddress, bool reuse_address)
		:
		local_address_(inetaddress),
		reuse_address_(reuse_address)
	{
		fd_ = ::socket(inetaddress.get_linux_family(), SOCK_STREAM, 0);
		EXCEPTION_GUARD_ERRNO(fd_ < 0, socketexception);

		const int flag = reuse_address_;
		int rc = setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
		EXCEPTION_GUARD_ERRNO(rc, socketexception);

		auto address = static_cast<struct sockaddr_in>(local_address_);

		rc = ::bind(fd_, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
		EXCEPTION_GUARD_ERRNO(rc, socketexception);

		rc = ::listen(fd_, MAX_QUEUED_REQUESTS);
		EXCEPTION_GUARD_ERRNO(rc, socketexception);
	}

} // namespace devfix::net::lnx


#endif
