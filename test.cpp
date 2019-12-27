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

  std::cout << "remote address: " << socket->get_remote_address().get_host() << ":" << socket->get_remote_address().port_ << std::endl;
  std::cout << "local address:  " << socket->get_local_address().get_host() << ":" << socket->get_local_address().port_ << std::endl;

  std::thread thread(
      [&socket]
      {
        char uff[8];
        devfix::base::io::inputstream &is = socket->get_inputstream();
        try
        {
          is.read(uff, sizeof(uff));

          long n;
          is.read(&n, sizeof(n));
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
