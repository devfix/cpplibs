//
// Created by core on 6/27/20.
//

#include "../platform.h"

#if PLATFORM_LINUX == 1

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "../filesystem.h"
#include "../error/ioexception.h"

#include <libgen.h>
#undef basename  // in <libgen.h> it is an alias for __xpg_basename

namespace devfix::base
{
	const char filesystem::SEPARATOR = '/';
	const std::string_view filesystem::SEPARATOR_SV = "/";

	static constexpr __mode_t DEFAULT_DIR_MODE = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
	static constexpr __mode_t DEFAULT_FILE_MODE = DEFFILEMODE;

	bool filesystem::exists(const std::string& filepath)
	{
		if (::access(filepath.c_str(), F_OK) == 0) { return true; }
		EXCEPTION_GUARD_ERRNO(errno != ENOENT, devfix::base::error::ioexception);
		return false;
	}

	bool filesystem::is_abs_path(const std::string& filepath)
	{
		// empty path is illegal
		EXCEPTION_GUARD_MSG(filepath.empty(), devfix::base::error::ioexception, "empty filepath");

		return filepath[0] == SEPARATOR;
	}

	bool filesystem::isfile(const std::string& filepath)
	{
		struct stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::stat(filepath.c_str(), &file_stat), devfix::base::error::ioexception);
		return S_ISREG(file_stat.st_mode);
	}

	void filesystem::touch(const std::string& filepath)
	{
		int file = ::open(filepath.c_str(), O_RDWR | O_CREAT, DEFAULT_FILE_MODE);
		EXCEPTION_GUARD_ERRNO (file < 0, devfix::base::error::ioexception);
		EXCEPTION_GUARD_ERRNO (::close(file), devfix::base::error::ioexception);
	}

	void filesystem::rm(const std::string& filepath)
	{
		EXCEPTION_GUARD_ERRNO(std::remove(filepath.c_str()), devfix::base::error::ioexception);
	}

	bool filesystem::isdir(const std::string& filepath)
	{
		struct stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::stat(filepath.c_str(), &file_stat), devfix::base::error::ioexception);
		return S_ISDIR(file_stat.st_mode);
	}

	void filesystem::mkdir(const std::string& filepath, bool parents)
	{
		if (parents)
		{
			std::string dir = dirname(filepath);
			if (!exists(dir)) { mkdir(dir, true); }
		}
		EXCEPTION_GUARD_ERRNO(::mkdir(filepath.c_str(), DEFAULT_DIR_MODE), devfix::base::error::ioexception);
	}

	std::string filesystem::basename(const std::string& filepath)
	{
		// empty path is illegal
		EXCEPTION_GUARD_MSG(filepath.empty(), devfix::base::error::ioexception, "empty filepath");

		std::string fp(filepath);  // create copy since ::basename can modify the argument
		return ::__xpg_basename(const_cast<char*>(fp.c_str()));
	}

	std::string filesystem::dirname(const std::string& filepath)
	{
		// empty path is illegal
		EXCEPTION_GUARD_MSG(filepath.empty(), devfix::base::error::ioexception, "empty filepath");

		// create copy of path and remove all trailing separators '/' except one
		std::string fp(filepath);
		if (*fp.rbegin() == SEPARATOR) { while (!fp.empty() && *fp.rbegin() == SEPARATOR) { fp.pop_back(); }}
		fp.push_back(SEPARATOR);

		return ::dirname(const_cast<char*>(fp.c_str()));
	}

	std::list<std::string> filesystem::lstdir(const std::string& filepath)
	{
		EXCEPTION_GUARD_MSG (!exists(filepath), devfix::base::error::ioexception, "filepath not found");
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
		else { EXCEPTION_GUARD_ERRNO(::rmdir(filepath.c_str()), devfix::base::error::ioexception); }
	}

	std::string filesystem::get_temp_dir()
	{
		const char* filepath = nullptr;
		if ((filepath = ::getenv("TMPDIR"))) { return filepath; }
		if ((filepath = ::getenv("TEMPDIR"))) { return filepath; }
		if ((filepath = ::getenv("TMP"))) { return filepath; }
		if ((filepath = ::getenv("TEMP"))) { return filepath; }
		return "/tmp";
	}
}

#endif
