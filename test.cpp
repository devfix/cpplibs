//
// Created by core on 12/19/19.
//

#include <thread>
#include <unistd.h>
#include <iostream>
#include "devfix/net/linux/lnx_socket.h"
#include "devfix/base/interruptedexception.h"
#include "devfix/net/netbuilder.h"

using namespace devfix::net;
using namespace devfix::base;

int main()
{
  auto socket = netbuilder::create_socket(inetaddress::create_by_host("localhost", 20000));

  std::thread thread(
      [&socket]
      {
        socket->set_out_buf_size(4);

        char uff[8];
        try
        {
          socket->read(uff, sizeof(uff));
        } catch (interruptedexception &interruptedexception)
        {
          std::cout << "interrupted" << std::endl;
        }
        asm("nop");
      });

  sleep(1);
  socket->set_interrupted(true);

  thread.join();
}
