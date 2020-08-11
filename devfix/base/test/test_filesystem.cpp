//
// Created by core on 5/25/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch2/catch.hpp>
#include "../platform.h"
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

TEST_CASE("FileSystem - IsDir / IsFile")
{
	prolog();
	REQUIRE(filesystem::isfile(tmp_dir + "a"));
	REQUIRE(filesystem::isfile(tmp_dir + "b"));
	REQUIRE(filesystem::isdir(tmp_dir + "c"));
	REQUIRE_FALSE(filesystem::isdir(tmp_dir + "a"));
	REQUIRE_FALSE(filesystem::isdir(tmp_dir + "b"));
	REQUIRE_FALSE(filesystem::isfile(tmp_dir + "c"));
	epilog();
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

	std::string ldir =dir + filesystem::SEPARATOR + "sub" + filesystem::SEPARATOR + "sub" + filesystem::SEPARATOR + "sub" + filesystem::SEPARATOR
		+ "sub";
	REQUIRE_NOTHROW(filesystem::mkdir(ldir, true));
	REQUIRE_NOTHROW(filesystem::mkdir(ldir, true));  // second time, now all directories exists
	REQUIRE(filesystem::exists(ldir));

	epilog();
}

TEST_CASE("FileSystem - List")
{
	prolog();
	auto list = filesystem::lstdir(tmp_dir);
	REQUIRE(list.size() == 3);
	REQUIRE(std::find(list.begin(), list.end(), "z") == list.end());
	CHECK_FALSE(std::find(list.begin(), list.end(), "a") == list.end());
	CHECK_FALSE(std::find(list.begin(), list.end(), "b") == list.end());
	CHECK_FALSE(std::find(list.begin(), list.end(), "c") == list.end());
	CHECK(filesystem::size(tmp_dir + "a") == 0);
	CHECK(filesystem::size(tmp_dir + "b") == 0);
	CHECK_THROWS(filesystem::size(tmp_dir + "c"));
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

TEST_CASE("FileSystem - IsAbsPath")
{
#if PLATFORM_LINUX == 1
	CHECK(filesystem::is_abs_path("/test/uff"));
	CHECK(filesystem::is_abs_path("/test"));
	CHECK(filesystem::is_abs_path("/"));
	CHECK_FALSE(filesystem::is_abs_path("dir/"));
#endif

#if PLATFORM_WINDOWS == 1
	CHECK(filesystem::is_abs_path("c:\\test\\uff"));
	CHECK(filesystem::is_abs_path("d:\\test"));
	CHECK(filesystem::is_abs_path("f:\\"));
	CHECK_FALSE(filesystem::is_abs_path("dir\\"));
#endif
}

TEST_CASE("FileSystem - Basename")
{
	const std::string dirname = std::string("c:") + filesystem::SEPARATOR + "root" + filesystem::SEPARATOR + "dir";
	CHECK_THROWS(filesystem::basename(""));
	CHECK(filesystem::basename(".") == ".");
	CHECK(filesystem::basename("..") == "..");
	CHECK(filesystem::basename("file") == "file");
	CHECK(filesystem::basename(filesystem::SEPARATOR_SV.data()) == filesystem::SEPARATOR_SV);
	CHECK(filesystem::basename(std::string("file") + filesystem::SEPARATOR_SV.data()) == "file");
	CHECK(filesystem::basename(std::string(filesystem::SEPARATOR_SV) + "file" + filesystem::SEPARATOR_SV.data()) == "file");
	CHECK(filesystem::basename(dirname + filesystem::SEPARATOR + "file") == "file");
	CHECK(filesystem::basename(dirname + filesystem::SEPARATOR + "file.ext") == "file.ext");
	CHECK(filesystem::basename(dirname + filesystem::SEPARATOR + "file.ext1.ext2") == "file.ext1.ext2");
}

TEST_CASE("FileSystem - Dirname")
{
	const std::string dirname = std::string("c:") + filesystem::SEPARATOR + "root" + filesystem::SEPARATOR + "dir";
	CHECK_THROWS(filesystem::dirname(""));
	CHECK(filesystem::dirname(".") == ".");
	CHECK(filesystem::dirname("..") == ".");
	CHECK(filesystem::dirname("file") == ".");
	CHECK(filesystem::dirname(filesystem::SEPARATOR_SV.data()) == filesystem::SEPARATOR_SV);
	CHECK(filesystem::dirname(std::string(filesystem::SEPARATOR_SV) + filesystem::SEPARATOR) == filesystem::SEPARATOR_SV);
	CHECK(filesystem::dirname(std::string(filesystem::SEPARATOR_SV) + filesystem::SEPARATOR + filesystem::SEPARATOR)
			  == filesystem::SEPARATOR_SV);
	CHECK(filesystem::dirname(std::string("file") + filesystem::SEPARATOR_SV.data()) == ".");
	CHECK(
		filesystem::dirname(std::string(filesystem::SEPARATOR_SV) + "file" + filesystem::SEPARATOR_SV.data()) == filesystem::SEPARATOR_SV);
	CHECK(filesystem::dirname(dirname + filesystem::SEPARATOR + "file") == dirname);
	CHECK(filesystem::dirname(dirname + filesystem::SEPARATOR + "file.ext") == dirname);
	CHECK(filesystem::dirname(dirname + filesystem::SEPARATOR + "file.ext1.ext2") == dirname);
}

#endif
