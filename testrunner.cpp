//
// Created by core on 06/28/20.
//

#define CATCH_CONFIG_EXTERNAL_INTERFACES
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int main(int argc, char* argv[])
{
	return Catch::Session().run(argc, argv);
}
