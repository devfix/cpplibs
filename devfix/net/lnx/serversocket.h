//
// Created by core on 12/29/19.
//


#pragma once

#include "../../base/platform.h"

#if PLATFORM_LINUX == 1

#include "../serversocket.h"


// forward declaration for external builder
namespace devfix::net
{
	struct [[maybe_unused]] netbuilder;
}

namespace devfix::net::lnx
{

struct serversocket : public devfix::net::serversocket
	{
		friend struct net::netbuilder;

		~serversocket() override;

		[[nodiscard]] std::unique_ptr<devfix::net::socket> accept() final;

		[[nodiscard]] const inetaddress& get_address() const final;

		[[nodiscard]] bool get_reuse_address() const final;

		void set_accept_timeout(devfix::net::socket::timeout_t timeout) final;
		[[nodiscard]] devfix::net::socket::timeout_t get_accept_timeout() const final;

		void close() final;
		[[nodiscard]] bool is_closed() const final;

	private:
		static constexpr int MAX_QUEUED_REQUESTS = 5;
		const inetaddress local_address_;
		bool reuse_address_ = false;
		int fd_ = -1;

		serversocket(inetaddress inetaddress, bool reuse_address);

	};

} // namespace devfix::net::lnx

#endif
