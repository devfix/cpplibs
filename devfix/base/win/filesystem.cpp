//
// Created by core on 6/27/20.
//

#include "../platform.h"

#if PLATFORM_WINDOWS == 1

#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <windows.h>
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

	bool filesystem::is_abs_path(const std::string& filepath)
	{
		// empty path is illegal
		EXCEPTION_GUARD_MSG(filepath.empty(), devfix::base::error::ioexception, "empty filepath");

		return filepath.length() >= 3 && filepath[1] == ':' && filepath[2] == SEPARATOR;
	}

	bool filesystem::isfile(const std::string& filepath)
	{
		std::string fp(filepath);
		while (!fp.empty() && *fp.rbegin() == SEPARATOR) { fp.pop_back(); }  // remove trailing '\'

		struct _stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::_stat(fp.c_str(), &file_stat), devfix::base::error::ioexception);
		return S_ISREG(file_stat.st_mode);
	}

	std::uint64_t filesystem::size(const std::string& filepath)
	{
		struct _stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::_stat(filepath.c_str(), &file_stat), devfix::base::error::ioexception);
		EXCEPTION_GUARD_MSG(!S_ISREG(file_stat.st_mode), devfix::base::error::ioexception, "is not regular file");
		return static_cast<uint64_t>(file_stat.st_size);
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
		std::string fp(filepath);
		while (!fp.empty() && *fp.rbegin() == SEPARATOR) { fp.pop_back(); }  // remove trailing '\'

		struct _stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::_stat(fp.c_str(), &file_stat), devfix::base::error::ioexception);
		return S_ISDIR(file_stat.st_mode);
	}

	void filesystem::mkdir(const std::string& filepath, bool parents)
	{
		if (parents)
		{
			std::string dir = dirname(filepath);
			if (!exists(dir)) { mkdir(dir, true); }

			if (exists(filepath))
			{
				EXCEPTION_GUARD_MSG(!isdir(filepath), devfix::base::error::ioexception, "file exists");
				return;
			}
		}
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
		//if (!is_abs_path(fp)) { fp = std::string(".") + fp; }

		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		[[maybe_unused]] char fname[_MAX_FNAME];
		[[maybe_unused]] char ext[_MAX_EXT];
		::_splitpath(fp.c_str(), drive, dir, fname, ext);
		std::string dname(drive);
		dname += dir;

		if (dname.empty()) { return "."; }
		if (dname.length() >= 2 && *dname.rbegin() == SEPARATOR) { dname.pop_back(); }  // remove last '\'
		return dname;
	}

	std::list<std::string> filesystem::lstdir(const std::string& filepath)
	{
		EXCEPTION_GUARD_MSG (!exists(filepath), devfix::base::error::ioexception, "filepath not found");
		EXCEPTION_GUARD_MSG(!isdir(filepath), devfix::base::error::ioexception, "filepath is not directory");


		std::string searchpath = filepath + SEPARATOR + "*.*";
		_WIN32_FIND_DATAA fd_file;
		HANDLE handle = ::FindFirstFileA(searchpath.c_str(), &fd_file);
		EXCEPTION_GUARD_MSG(handle == INVALID_HANDLE_VALUE, devfix::base::error::ioexception, "filepath not found");

		std::list<std::string> entries;
		do
		{
			const char* name = fd_file.cFileName;
			std::size_t len = std::strlen(name);

			// only add real file name who are not "." or ".."
			if ((len != 1 || name[0] != '.') && (len != 2 || name[0] != '.' || name[1] != '.')) { entries.emplace_back(name); }
		} while (::FindNextFileA(handle, &fd_file));

		::FindClose(handle);
		return entries;
	}

	void filesystem::rmdir(const std::string& filepath, bool recursive)
	{
		if (recursive)
		{
			const auto list = lstdir(filepath);
			for (const auto& entry : list)
			{
				std::string entrypath = filepath;
				entrypath += SEPARATOR;
				entrypath += entry;
				if (isdir(entrypath)) { rmdir(entrypath, true); }
				else { rm(entrypath); }
			}
			rmdir(filepath, false);
		}
		else { EXCEPTION_GUARD_ERRNO(::_rmdir(filepath.c_str()), devfix::base::error::ioexception); }
	}

	std::string filesystem::get_temp_dir() noexcept
	{
		char* filepath = nullptr;
		if ((filepath = ::getenv("TEMP"))) { return filepath; }
		if ((filepath = ::getenv("TMP"))) { return filepath; }
		throw devfix::base::error::ioexception("could not get temp dir");
	}

	std::string filesystem::get_cache_dir() noexcept
	{
		const char* filepath = nullptr;
		if ((filepath = ::getenv("LOCALAPPDATA"))) { return std::string(filepath) + SEPARATOR + "cache"; }
		return get_temp_dir() + SEPARATOR + "cache";
	}

}

#endif
