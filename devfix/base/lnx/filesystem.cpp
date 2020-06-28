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
#include <stdexcept>
#include "../filesystem.h"
#include "../error/ioexception.h"

namespace devfix::base
{
	const char filesystem::SEPARATOR = '/';

	static constexpr __mode_t DEFAULT_LINUX_FILEMODE = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;

	bool filesystem::exists(const std::string& filepath)
	{
		return ::access(filepath.c_str(), F_OK) != -1;
	}

	bool filesystem::isfile(const std::string& filepath)
	{
		struct stat file_stat{};
		EXCEPTION_GUARD_ERRNO(::stat(filepath.c_str(), &file_stat), devfix::base::error::ioexception);
		return S_ISREG(file_stat.st_mode);
	}

	void filesystem::touch(const std::string& filepath)
	{
		int file = ::open(filepath.c_str(), O_RDWR | O_CREAT, DEFFILEMODE);
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

	void filesystem::mkdir(const std::string& filepath)
	{
		EXCEPTION_GUARD_ERRNO(::mkdir(filepath.c_str(), DEFAULT_LINUX_FILEMODE), devfix::base::error::ioexception);
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
