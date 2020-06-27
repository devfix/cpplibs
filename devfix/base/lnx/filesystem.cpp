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
		if (::stat(filepath.c_str(), &file_stat)) { throw std::runtime_error(std::strerror(errno)); }
		return S_ISREG(file_stat.st_mode);
	}

	void filesystem::touch(const std::string& filepath)
	{
		int file = ::open(filepath.c_str(), O_RDWR | O_CREAT, DEFFILEMODE);
		if (file < 0) { throw std::runtime_error(std::strerror(errno)); }
		if (::close(file)) { throw std::runtime_error(std::strerror(errno)); }
	}

	void filesystem::rm(const std::string& filepath)
	{
		if (std::remove(filepath.c_str())) { throw std::runtime_error(std::strerror(errno)); }
	}

	bool filesystem::isdir(const std::string& filepath)
	{
		struct stat file_stat{};
		if (::stat(filepath.c_str(), &file_stat)) { throw std::runtime_error(std::strerror(errno)); }
		return S_ISDIR(file_stat.st_mode);
	}

	void filesystem::mkdir(const std::string& filepath)
	{
		if (::mkdir(filepath.c_str(), DEFAULT_LINUX_FILEMODE)) { throw std::runtime_error(std::strerror(errno)); }
	}

	std::list<std::string> filesystem::lstdir(const std::string& filepath)
	{
		if (!exists(filepath)) { throw std::invalid_argument("filepath not found"); }
		if (!isdir(filepath)) { throw std::invalid_argument("filepath is not directory"); }

		std::list<std::string> entries;
		DIR* dir = nullptr;
		struct dirent* entry;
		dir = ::opendir(filepath.c_str());
		if (dir)
		{
			while ((entry = ::readdir(dir)))
			{
				const char* name = entry->d_name;
				std::size_t len = std::strlen(name);

				// only add real file name who are not "." or ".."
				if ((len != 1 || name[0] != '.') && (len != 2 || name[0] != '.' || name[1] != '.')) { entries.emplace_back(name); }
			}
			::closedir(dir);
		}
		else { throw std::runtime_error(std::strerror(errno)); }
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
		else
		{
			if (::rmdir(filepath.c_str())) { throw std::runtime_error(std::strerror(errno)); }
		}
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
