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

	/**
	 * \brief Class for management and conversion of internet addresses.
	 */
	struct inetaddress
	{

		typedef std::uint32_t address_t; //!< ipv4 internet address

		typedef std::uint16_t port_t; //!< protocol port

		/**
		 * \brief Supported underlying protocols.
		 */
		enum class family_t : char
		{
			UNSPECIFIED = 0,
			IPV4 = 1
		};

	private:
		address_t address_ = { 0 };
		port_t port_ = 0;
		family_t family_ = family_t::UNSPECIFIED;

	public:
		/**
		 * \brief Creates a default <i>inetaddress</i>, zeroed address, port and unspecified family.
		 */
		inetaddress() = default;

		/**
		 * \brief Creates an <i>inetaddress</i> by given hostname (dns or ip address).
		 * \param host hostname, gets converted to ipv4
		 * \param port protocol port
		 * \param family underlying protocol
		 */
		inetaddress(const std::string& host, port_t port, family_t family = family_t::IPV4);

#if PLATFORM_LINUX == 1
		/**
		 * \brief Linux only: Constructor to create an <i>inetaddress</i> from a <i>sockadr_in</i>.
		 * \param sockaddr linux' internet address
		 */
		explicit inetaddress(const struct sockaddr_in& sockaddr);

		/**
		 * \brief Linux only: Conversion operator to create a <i>sockadr_in</i> from an <i>inetaddress</i>.
		 * \return linux' internet address (sockadr_in)
		 */
		explicit operator sockaddr_in() const;

		/**
		 * \brief Linux only: Get family as sa_family_t.
		 * \return sa_family_t
		 */
		[[nodiscard]] sa_family_t get_linux_family() const;

		/**
		 * \brief Linux only: Drz family from sa_family_t.
		 * \param address_family linux family
		 */
		void set_linux_family(sa_family_t address_family);
#endif

		/**
		 * \brief Convert binary host address to string.
		 * \return address as string
		 */
		[[nodiscard]] std::string get_host() const noexcept;

		/**
		 * \return binary address
		 */
		[[nodiscard]] address_t get_address() const;

		/**
		 * \return protocol port
		 */
		[[nodiscard]] port_t get_port() const;

		/**
		 * \return family of underlying protocol
		 */
		[[nodiscard]] family_t get_family() const;
	};

} // namespace devfix::net
