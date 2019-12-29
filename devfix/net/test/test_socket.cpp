//
// Created by core on 12/19/19.
//

#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "../netbuilder.h"

using namespace devfix::net;
using namespace devfix::base;

constexpr inetaddress::port_t TEST_PORT = 30000;
constexpr std::size_t REPETITIONS = 1000;
constexpr bool MUTE = false;

constexpr long TEST_LONG = 1000000;
constexpr float TEST_FLOAT = 3.1415f;

TEST(TestSocket, Address)
{
  std::atomic_bool server_ready = false;
  inetaddress server_local_address{};
  inetaddress server_remote_address{};

  std::thread server_thread(
      [&server_ready, &server_local_address, &server_remote_address]()
      {
        auto server = netbuilder::create_serversocket(
            {0, TEST_PORT, inetaddress::family::IPV4},
            true
        );
        server_ready = true;

        auto client = server->accept();
        server_local_address = client->get_local_address();
        server_remote_address = client->get_remote_address();
      });

  while(!server_ready);

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

/*
void test_io()
{
  std::atomic_bool server_ready = false;

  std::thread server_thread(
      [&server_ready]()
      {
        auto server = netbuilder::create_serversocket(
            {0, TEST_PORT, inetaddress::family::IPV4},
            true
        );
        server_ready = true;

        auto client = server->accept();
        io::inputstream &is = client->get_inputstream();
        io::outputstream &os = client->get_outputstream();

        long d;
        is.read(&d, sizeof(d));

        assert(d == TEST_LONG);

        float f = 3.1415f;
        os.write(&f, sizeof(f));

        // no close needed
      });

  while(!server_ready);

  auto client = netbuilder::create_socket(inetaddress::create_by_host("localhost", TEST_PORT));
  io::inputstream &is = client->get_inputstream();
  io::outputstream &os = client->get_outputstream();

  long d = TEST_LONG;
  os.write(&d, sizeof(d));

  float f;
  is.read(&f, sizeof(f));
  assert(f == TEST_FLOAT);

  server_thread.join();
}

int maiin()
{
  for (std::size_t i = 0; i < REPETITIONS; i++)
  {
    test_io();
  }
  std::cout << "Test IO successful." << std::endl;

}*/
