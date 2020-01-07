//
// Created by core on 12/18/19.
//


#pragma once

#include "../base/platform.h"

#include <string>

#if PLATFORM_LINUX == 1
#include <netinet/in.h>
#endif

namespace devfix::net
{

	struct inetaddress
	{

		typedef std::uint32_t address_t;

		typedef std::uint16_t port_t;

		enum class family_t : char
		{
			UNSUPPORTED = 0,
			IPV4 = 1
		};

	private:
		address_t address_ = { 0 };
		port_t port_ = 0;
		family_t family_ = family_t::UNSUPPORTED;

	public:
		inetaddress() = default;
		inetaddress(const std::string& host, port_t port, family_t family = family_t::IPV4);

#if PLATFORM_LINUX == 1
		explicit inetaddress(const struct sockaddr_in& sockaddr);
		operator sockaddr_in() const;
		sa_family_t get_linux_family() const;
		void set_linux_family(sa_family_t address_family);
#endif

		[[nodiscard]] std::string get_host() const noexcept;
		[[nodiscard]] address_t get_address() const;
		[[nodiscard]] port_t get_port() const;
		[[nodiscard]] family_t get_family() const;
	};

} // namespace devfix::net
