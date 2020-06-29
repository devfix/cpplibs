//
// Created by core on 6/27/20.
//

#include "../platform.h"

#if PLATFORM_WINDOWS == 1

#include <cstring>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../filesystem.h"
#include "../error/ioexception.h"

namespace devfix::base
{
	const char filesystem::SEPARATOR = '\\';
	const std::string_view filesystem::SEPARATOR_SV = "\\";

	bool filesystem::exists(const std::string& filepath)
	{
		if (::_access(filepath.c_str(), 0) == 0) { return true; }
		EXCEPTION_GUARD_ERRNO(errno != ENOENT, devfix::base::error::ioexception);
		return false;
	}

	bool filesystem::isfile(const std::string& filepath)
	{
		struct _stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::_stat(filepath.c_str(), &file_stat), devfix::base::error::ioexception);
		return S_ISREG(file_stat.st_mode);
	}

	void filesystem::touch(const std::string& filepath)
	{
		int file = ::_open(filepath.c_str(), _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE);
		EXCEPTION_GUARD_ERRNO (file < 0, devfix::base::error::ioexception);
		EXCEPTION_GUARD_ERRNO (::close(file), devfix::base::error::ioexception);
	}

	void filesystem::rm(const std::string& filepath)
	{
		EXCEPTION_GUARD_ERRNO(std::remove(filepath.c_str()), devfix::base::error::ioexception);
	}

	bool filesystem::isdir(const std::string& filepath)
	{
		struct _stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::_stat(filepath.c_str(), &file_stat), devfix::base::error::ioexception);
		return S_ISDIR(file_stat.st_mode);
	}

	void filesystem::mkdir(const std::string& filepath, bool parents)
	{
		EXCEPTION_GUARD_ERRNO(::_mkdir(filepath.c_str()), devfix::base::error::ioexception);
	}

	std::string filesystem::basename(const std::string& filepath)
	{
		// empty path is illegal
		EXCEPTION_GUARD_MSG(filepath.empty(), devfix::base::error::ioexception, "empty filepath");

		// create copy of path and remove all trailing separators '\'
		std::string fp(filepath);
		while (!fp.empty() && *fp.rbegin() == SEPARATOR) { fp.pop_back(); }

		if (fp.empty()) { return SEPARATOR_SV.data(); }  // if path is now empty string, it consisted only of backslashes

		// decompose path
		[[maybe_unused]] char drive[_MAX_DRIVE];
		[[maybe_unused]] char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		::_splitpath(fp.c_str(), drive, dir, fname, ext);
		return std::string(fname) + ext;
	}

	std::string filesystem::dirname(const std::string& filepath)
	{
		// empty path is illegal
		EXCEPTION_GUARD_MSG(filepath.empty(), devfix::base::error::ioexception, "empty filepath");

		// create copy of path and remove all trailing separators '\'
		std::string fp(filepath);
		while (!fp.empty() && *fp.rbegin() == SEPARATOR) { fp.pop_back(); }

		if (fp.empty()) { return SEPARATOR_SV.data(); }  // if path is now empty string, it consisted only of backslashes
		if (fp == "." || fp == "..") { return "."; }

		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		[[maybe_unused]] char fname[_MAX_FNAME];
		[[maybe_unused]] char ext[_MAX_EXT];
		::_splitpath(fp.c_str(), drive, dir, fname, ext);
		if (std::size_t p = std::strlen(dir); p) { dir[p-1] = '\0'; }  // remove last '\'
		return std::string(drive) + dir;
	}

	std::list<std::string> filesystem::lstdir(const std::string& filepath)
	{
		/*EXCEPTION_GUARD_MSG (!exists(filepath), devfix::base::error::ioexception, "filepath not found");
		EXCEPTION_GUARD_MSG(!isdir(filepath), devfix::base::error::ioexception, "filepath is not directory");

		std::list<std::string> entries;
		DIR* dir = nullptr;
		struct dirent* entry;
		EXCEPTION_GUARD_ERRNO(!(dir = ::opendir(filepath.c_str())), devfix::base::error::ioexception);
		while ((entry = ::readdir(dir)))
		{
			const char* name = entry->d_name;
			std::size_t len = std::strlen(name);

			// only add real file name who are not "." or ".."
			if ((len != 1 || name[0] != '.') && (len != 2 || name[0] != '.' || name[1] != '.')) { entries.emplace_back(name); }
		}
		EXCEPTION_GUARD_ERRNO(::closedir(dir), devfix::base::error::ioexception);
		return entries;*/
	}

	void filesystem::rmdir(const std::string& filepath, bool recursive)
	{
		throw devfix::base::error::ioexception("not implemented 8");
	}

	std::string filesystem::get_temp_dir()
	{
		char* filepath = nullptr;
		if ((filepath = ::getenv("TEMP"))) { return filepath; }
		if ((filepath = ::getenv("TMP"))) { return filepath; }
		throw devfix::base::error::ioexception("could not get temp dir");
	}

}

#endif
