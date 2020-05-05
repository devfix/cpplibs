//
// Created by core on 12/29/19.
//

#if ENABLE_GOOGLETEST == 1
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#else

#pragma message ("Googletest is disabled.")

#include <iostream>
#include "devfix/net/inetaddress.h"

int main()
{
	std::cout << "testrunner stub: googletest is disabled." << std::endl;
	devfix::net::inetaddress i;
	std::string d = i.get_host();
}

#endif