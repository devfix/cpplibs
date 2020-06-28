//
// Created by core on 5/25/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch/catch.hpp>
#include "../filesystem.h"

using namespace devfix::base;

const std::string tmp_dir = filesystem::get_temp_dir() + filesystem::SEPARATOR + "cpplibs-test" + filesystem::SEPARATOR;

void prolog()
{
	if (!filesystem::exists(tmp_dir)) { filesystem::mkdir(tmp_dir); }
	if (!filesystem::exists(tmp_dir + "a")) { filesystem::touch(tmp_dir + "a"); }
	if (!filesystem::exists(tmp_dir + "b")) { filesystem::touch(tmp_dir + "b"); }
	if (!filesystem::exists(tmp_dir + "c")) { filesystem::mkdir(tmp_dir + "c"); }
}

void epilog()
{
	if (filesystem::exists(tmp_dir)) { filesystem::rmdir(tmp_dir, true); }
}

TEST_CASE("FileSystem - Directories")
{
#if PLATFORM_LINUX == 1
	REQUIRE_THROWS(filesystem::mkdir("/bad-permission-test"));
#endif
	prolog();
	const std::string dir = tmp_dir + filesystem::SEPARATOR + "dir";

	REQUIRE_FALSE(filesystem::exists(dir));
	REQUIRE_THROWS(static_cast<void>(filesystem::isdir(dir)));
	REQUIRE_THROWS(static_cast<void>(filesystem::isfile(dir)));
	REQUIRE_NOTHROW(filesystem::mkdir(dir));
	REQUIRE(filesystem::exists(dir));
	REQUIRE(filesystem::isdir(dir));
	REQUIRE_FALSE(filesystem::isfile(dir));
	REQUIRE_NOTHROW(filesystem::rmdir(dir));
	REQUIRE_FALSE(filesystem::exists(dir));
	REQUIRE_THROWS(filesystem::rmdir(dir));

	epilog();
}

TEST_CASE("FileSystem - List")
{
	prolog();
	auto list = filesystem::lstdir(tmp_dir);
	REQUIRE(list.size() == 3);
	REQUIRE(std::find(list.begin(), list.end(), "z") == list.end());
	REQUIRE_FALSE(std::find(list.begin(), list.end(), "a") == list.end());
	REQUIRE_FALSE(std::find(list.begin(), list.end(), "b") == list.end());
	REQUIRE_FALSE(std::find(list.begin(), list.end(), "c") == list.end());
	epilog();
}

TEST_CASE("FileSystem - RecursiveDelete")
{
	prolog();
	filesystem::mkdir(tmp_dir + filesystem::SEPARATOR + "d");
	filesystem::mkdir(tmp_dir + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d");
	filesystem::mkdir(tmp_dir + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d");
	filesystem::touch(tmp_dir + filesystem::SEPARATOR + "f");
	filesystem::touch(tmp_dir + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "f");
	filesystem::touch(tmp_dir + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "f");
	filesystem::touch(tmp_dir +
		filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "f");
	REQUIRE(filesystem::exists(
		tmp_dir + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "f"));

	filesystem::rmdir(tmp_dir, true);

	REQUIRE_FALSE(filesystem::exists(tmp_dir));
	epilog();
}

#endif
