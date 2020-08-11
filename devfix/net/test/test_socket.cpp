//
// Created by core on 12/19/19.
//

#if CPPLIBS_ENABLE_NET_TESTS == 1

#include <catch2/catch.hpp>
#include <thread>
#include <unistd.h>
#include <cstring>
#include "../netbuilder.h"
#include "../socketexception.h"
#include "../../base/error/timeoutexception.h"

using namespace devfix::net;
using namespace devfix::base;

constexpr inetaddress::port_t TEST_PORT = 30000;

constexpr long TEST_LONG = 1000000;

constexpr float TEST_FLOAT = 3.1415f;

constexpr double TEST_DOUBLE = 1.4142;

constexpr std::array<float, 4> TEST_ARRAY = { 1.0, 1.1, 1.2, 1.3 };

TEST_CASE("Socket - Address")
{
	std::atomic_bool server_ready = false;
	inetaddress server_local_address{};
	inetaddress server_remote_address{};

	std::thread server_thread(
		[&server_ready, &server_local_address, &server_remote_address]()
		{
			auto server = netbuilder::create_serversocket(
				{ "0.0.0.0", TEST_PORT, inetaddress::family_t::IPV4 },
				true
														 );
			server_ready = true;

			auto client = server->accept();
			server_local_address = client->get_local_address();
			server_remote_address = client->get_remote_address();
		});

	while (!server_ready) {}

	auto client = netbuilder::create_socket(inetaddress("localhost", TEST_PORT));

	const inetaddress& local_address = client->get_local_address();
	const inetaddress& remote_address = client->get_remote_address();

	server_thread.join();

	REQUIRE(local_address.get_port() == server_remote_address.get_port());
	REQUIRE(remote_address.get_port() == server_local_address.get_port());
	REQUIRE(local_address.get_address() == server_remote_address.get_address());
	REQUIRE(remote_address.get_address() == server_local_address.get_address());
	REQUIRE(local_address.get_host() == server_remote_address.get_host());
	REQUIRE(remote_address.get_host() == server_local_address.get_host());
}

TEST_CASE("Socket - IO")
{
	std::atomic_bool server_ready = false;

	std::thread server_thread(
		[&server_ready]()
		{
			auto server = netbuilder::create_serversocket(
				{ "0.0.0.0", TEST_PORT, inetaddress::family_t::IPV4 },
				true
														 );
			server_ready = true;

			auto client = server->accept();
			auto& is = client->get_inputstream();
			auto& os = client->get_outputstream();

			long l;
			is.read(&l, sizeof(l));
			REQUIRE(l == TEST_LONG);

			float f = TEST_FLOAT;
			os.write(&f, sizeof(f));

			double d;
			is.read(&d, sizeof(d));
			REQUIRE(d == TEST_DOUBLE);

			os.write(&TEST_ARRAY, sizeof(TEST_ARRAY));

			// test stream close
			is.close();
			os.close();
		});

	while (!server_ready) {}

	auto client = netbuilder::create_socket(inetaddress("localhost", TEST_PORT));
	auto& is = client->get_inputstream();
	auto& os = client->get_outputstream();

	long l = TEST_LONG;
	os.write(&l, sizeof(l));

	float f;
	is.read(&f, sizeof(f));
	REQUIRE(f == TEST_FLOAT);

	double d = TEST_DOUBLE;
	os.write(&d, sizeof(d));

	char vector[sizeof(TEST_ARRAY)] = { 0 };
	is.read(vector, sizeof(TEST_ARRAY));
	REQUIRE(std::memcmp(vector, &TEST_ARRAY, sizeof(TEST_ARRAY)) == 0);

	// test stream close
	is.close();
	os.close();

	server_thread.join();
}

TEST_CASE("Socket - ReadTimeout")
{
	std::atomic_bool server_active = false;

	std::thread server_thread(
		[&server_active]()
		{
			auto server = netbuilder::create_serversocket(
				{ "0.0.0.0", TEST_PORT, inetaddress::family_t::IPV4 },
				true);
			server_active = true;
			auto client = server->accept();

			for (int k = 0; k < 3000 && server_active; k++) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
		});

	while (!server_active) {}

	auto client = netbuilder::create_socket(inetaddress("localhost", TEST_PORT));
	client->set_timeout(10);

	auto& is = client->get_inputstream();

	char uff;
	REQUIRE_THROWS(is.read(&uff, sizeof(uff)));

	server_active = false;
	server_thread.join();
}

TEST_CASE("Socket - WriteTimeout")
{
	std::atomic_bool server_active = false;

	std::thread server_thread(
		[&server_active]()
		{
			auto server = netbuilder::create_serversocket(
				{ "0.0.0.0", TEST_PORT, inetaddress::family_t::IPV4 },
				true);
			server_active = true;
			auto client = server->accept();

			for (int k = 0; k < 3000 && server_active; k++) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
		});

	while (!server_active) {}

	auto client = netbuilder::create_socket(inetaddress("localhost", TEST_PORT));
	client->set_timeout(10);

	auto& os = client->get_outputstream();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	char data[8192];
	bool timeout = false;
	while (!timeout)
	{
		try { os.write(&data, sizeof(data)); }
		catch (error::timeoutexception& timeoutexception) { timeout = true; }
	}

	REQUIRE(timeout);

	server_active = false;
	server_thread.join();
}

#endif
