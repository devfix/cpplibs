//
// Created by core on 12/29/19.
//

#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include <unistd.h>
#include "lnx_serversocket.h"
#include "../../base/error/baseexception.h"
#include "../socketexception.h"
#include "lnx_socket.h"

namespace devfix::net::lnx
{

	lnx_serversocket::~lnx_serversocket()
	{
		close();
	}

	std::unique_ptr<socket> lnx_serversocket::accept()
	{
		struct sockaddr_in sockaddr_in{};
		socklen_t socklen = sizeof(sockaddr_in);
		int fd = ::accept(fd_, reinterpret_cast<sockaddr*>(&sockaddr_in), &socklen);
		exception_guard(fd < 0, socketexception);

		inetaddress remote_address(sockaddr_in);
		return std::unique_ptr<socket>(new lnx_socket(fd, remote_address));
	}

	const inetaddress& lnx_serversocket::get_address() const noexcept
	{
		return local_address_;
	}

	bool lnx_serversocket::get_reuse_address() const noexcept
	{
		return reuse_address_;
	}

	void lnx_serversocket::set_accept_timeout([[maybe_unused]] socket::timeout_t timeout)
	{
		exception_guard_m(true, socketexception, "This serversocket implementation does currently not support accept() with timeout.");
	}

	socket::timeout_t lnx_serversocket::get_accept_timeout() const noexcept
	{
		exception_guard_m(true, socketexception, "This serversocket implementation does currently not support accept() with timeout.");
	}

	void lnx_serversocket::close()
	{
		if (is_closed())
		{
			return;
		}

		int rc = ::close(fd_);
		fd_ = -1;
		exception_guard(rc, socketexception);
	}

	bool lnx_serversocket::is_closed() const noexcept
	{
		return fd_ < 0;
	}

	lnx_serversocket::lnx_serversocket(inetaddress inetaddress, bool reuse_address)
		:
		local_address_(inetaddress),
		reuse_address_(reuse_address)
	{
		fd_ = ::socket(inetaddress.get_linux_family(), SOCK_STREAM, 0);
		exception_guard(fd_ < 0, socketexception);

		const int flag = reuse_address_;
		int rc = setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
		exception_guard(rc, socketexception);

		auto address = static_cast<struct sockaddr_in>(local_address_);

		rc = ::bind(fd_, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
		exception_guard(rc, socketexception);

		rc = ::listen(fd_, MAX_QUEUED_REQUESTS);
		exception_guard(rc, socketexception);
	}

} // namespace devfix::net::lnx


#endif
