//
// Created by core on 12/19/19.
//


#pragma once

#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include <atomic>
#include "../socket.h"
#include "../inetaddress.h"
#include "../../base/io/source.h"
#include "../../base/io/sink.h"


// forward declaration for constructor call in netbuilder
namespace devfix::net {  struct [[maybe_unused]] netbuilder; }

namespace devfix::net::lnx
{
	struct serversocket; //!< forward declaration for accept() in lnx_serversocket

	struct socket : public devfix::net::socket
	{
		friend struct devfix::net::netbuilder; //!< friend declaration for constructor call in netbuilder
		friend struct devfix::net::lnx::serversocket; //!< friend declaration for accept() in lnx_serversocket

		~socket() final;

		[[nodiscard]] const inetaddress& get_local_address() const noexcept final;
		[[nodiscard]] const inetaddress& get_remote_address() const noexcept final;

		[[nodiscard]] base::io::inputstream& get_inputstream() const noexcept final;
		[[nodiscard]] base::io::outputstream& get_outputstream() const noexcept final;

		void set_interrupted(bool interrupted) noexcept final;
		[[nodiscard]] bool get_interrupted() const noexcept final;

		void set_timeout(timeout_t timeout) noexcept final;
		[[nodiscard]] timeout_t get_timeout() const noexcept final;

	private:
		int fd_ = -1;
		inetaddress remote_address_;
		inetaddress local_address_;
		std::atomic_bool interrupted_ = false;
		timeout_t timeout_ = DEFAULT_TIMEOUT;
		std::unique_ptr<base::io::source> source_;
		std::unique_ptr<base::io::sink> sink_;

		explicit socket(inetaddress remote_address);
		socket(int fd, inetaddress remote_address);

		[[nodiscard]] inetaddress _get_local_address() const;
		void _configure_io_timeout(int optname, timeout_t timeout);
		void _read(void* buf, std::size_t len);
		void _write(const void* buf, std::size_t len);
		void _skip(std::size_t n);
		void _flush();
		std::size_t _available();
		void _close();
		bool _is_closed();
	};

} // namespace devfix::net::lnx

#endif
