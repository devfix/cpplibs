//
// Created by core on 5/25/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
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

TEST(FileSystem, Directories)
{
#if PLATFORM_LINUX == 1
	ASSERT_ANY_THROW(filesystem::mkdir("/bad-permission-test"));
#endif
	prolog();
	const std::string dir = tmp_dir + filesystem::SEPARATOR + "dir";

	ASSERT_FALSE(filesystem::exists(dir));
	ASSERT_ANY_THROW(filesystem::isdir(dir));
	ASSERT_ANY_THROW(filesystem::isfile(dir));
	ASSERT_NO_THROW(filesystem::mkdir(dir));
	ASSERT_TRUE(filesystem::exists(dir));
	ASSERT_TRUE(filesystem::isdir(dir));
	ASSERT_FALSE(filesystem::isfile(dir));
	ASSERT_NO_THROW(filesystem::rmdir(dir));
	ASSERT_FALSE(filesystem::exists(dir));
	ASSERT_ANY_THROW(filesystem::rmdir(dir));

	epilog();
}

TEST(FileSystem, List)
{
	prolog();
	auto list = filesystem::lstdir(tmp_dir);
	ASSERT_EQ(list.size(), 3);
	ASSERT_TRUE(std::find(list.begin(), list.end(), "z") == list.end());
	ASSERT_FALSE(std::find(list.begin(), list.end(), "a") == list.end());
	ASSERT_FALSE(std::find(list.begin(), list.end(), "b") == list.end());
	ASSERT_FALSE(std::find(list.begin(), list.end(), "c") == list.end());
	epilog();
}

TEST(FileSystem, RecursiveDelete)
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
	ASSERT_TRUE(filesystem::exists(
		tmp_dir + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "d" + filesystem::SEPARATOR + "f"));

	filesystem::rmdir(tmp_dir, true);

	ASSERT_FALSE(filesystem::exists(tmp_dir));
	epilog();
}

#endif
