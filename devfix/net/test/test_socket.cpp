//
// Created by core on 12/19/19.
//

#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "../netbuilder.h"

using namespace devfix::net;
using namespace devfix::base;

constexpr inetaddress::port_t TEST_PORT = 30000;
constexpr long TEST_LONG = 1000000;
constexpr float TEST_FLOAT = 3.1415f;
constexpr double TEST_DOUBLE = 1.4142;

constexpr std::array<float, 4> TEST_ARRAY = {1.0, 1.1, 1.2, 1.3};

TEST(Socket, Address) {
    std::atomic_bool server_ready = false;
    inetaddress server_local_address{};
    inetaddress server_remote_address{};

    std::thread server_thread(
            [&server_ready, &server_local_address, &server_remote_address]() {
                auto server = netbuilder::create_serversocket(
                        {0, TEST_PORT, inetaddress::family::IPV4},
                        true
                );
                server_ready = true;

                auto client = server->accept();
                server_local_address = client->get_local_address();
                server_remote_address = client->get_remote_address();
            });

    while (!server_ready);

    auto client = netbuilder::create_socket(inetaddress::create_by_host("localhost", TEST_PORT));

    const inetaddress &local_address = client->get_local_address();
    const inetaddress &remote_address = client->get_remote_address();

    server_thread.join();

    ASSERT_EQ(local_address.port_, server_remote_address.port_);
    ASSERT_EQ(remote_address.port_, server_local_address.port_);
    ASSERT_EQ(local_address.address_.s_addr, server_remote_address.address_.s_addr);
    ASSERT_EQ(remote_address.address_.s_addr, server_local_address.address_.s_addr);
    ASSERT_EQ(local_address.get_host(), server_remote_address.get_host());
    ASSERT_EQ(remote_address.get_host(), server_local_address.get_host());
}

TEST(Socket, IO) {
    std::atomic_bool server_ready = false;
    inetaddress server_local_address{};
    inetaddress server_remote_address{};

    std::thread server_thread(
            [&server_ready, &server_local_address, &server_remote_address]() {
                auto server = netbuilder::create_serversocket(
                        {0, TEST_PORT, inetaddress::family::IPV4},
                        true
                );
                server_ready = true;

                auto client = server->accept();
                auto &is = client->get_inputstream();
                auto &os = client->get_outputstream();

                long l;
                is.read(&l, sizeof(l));
                ASSERT_EQ(l, TEST_LONG);

                float f = TEST_FLOAT;
                os.write(&f, sizeof(f));

                double d;
                is.read(&d, sizeof(d));
                ASSERT_EQ(d, TEST_DOUBLE);

                os.write(&TEST_ARRAY, sizeof(TEST_ARRAY));
            });

    while (!server_ready);

    auto client = netbuilder::create_socket(inetaddress::create_by_host("localhost", TEST_PORT));
    auto &is = client->get_inputstream();
    auto &os = client->get_outputstream();

    long l = TEST_LONG;
    os.write(&l, sizeof(l));

    float f;
    is.read(&f, sizeof(f));
    ASSERT_EQ(f, TEST_FLOAT);

    double d = TEST_DOUBLE;
    os.write(&d, sizeof(d));

    char vector[sizeof(TEST_ARRAY)] = {0};
    is.read(vector, sizeof(TEST_ARRAY));
    ASSERT_EQ(std::memcmp(vector, &TEST_ARRAY, sizeof(TEST_ARRAY)) ,0);

    server_thread.join();
}
