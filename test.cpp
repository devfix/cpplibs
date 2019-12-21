//
// Created by core on 12/19/19.
//

#include <thread>
#include <unistd.h>
#include <iostream>
#include "devfix/net/linux/lnx_socket.h"
#include "devfix/base/interruptedexception.h"

using namespace devfix::net;
using namespace devfix::util;

int main()
{
  lnx::lnx_socket linuxsocket(inetaddress::create_by_host(inetaddress::family::IPV4, 20000, "localhost"));
  std::thread thread(
      [&linuxsocket]
      {
        linuxsocket.set_out_buf_size(4);

        char uff[8];
        try
        {
          linuxsocket.read(uff, sizeof(uff));
        } catch (interruptedexception &interruptedexception)
        {
          std::cout << "interrupted" << std::endl;
        }
        asm("nop");
      });

  sleep(1);
  linuxsocket.set_interrupted(true);

  thread.join();
}
