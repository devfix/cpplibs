//
// Created by core on 6/27/20.
//

#pragma once

#include <list>
#include "platform.h"

#if PLATFORM_LINUX == 1
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#endif

namespace devfix::base
{
	struct filesystem
	{
#if PLATFORM_LINUX == 1
		static constexpr char SEPARATOR = '/';
		static constexpr __mode_t DEFAULT_LINUX_FILEMODE = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
#else
#pragma message ("Platform not supported")
#endif

		static bool exists(const std::string& filepath)
		{
#if PLATFORM_LINUX == 1
			return ::access(filepath.c_str(), F_OK) != -1;
#else
#pragma message ("Platform not supported")
#endif
		}

		static bool isfile(const std::string& filepath)
		{
#if PLATFORM_LINUX == 1
			struct stat file_stat;
			if (::stat(filepath.c_str(), &file_stat))
			{
				throw std::runtime_error(std::strerror(errno));
			}
			return S_ISREG(file_stat.st_mode);
#else
#pragma message ("Platform not supported")
#endif
		}

		static void touch(const std::string& filepath)
		{
#if PLATFORM_LINUX == 1
			int file = ::open(filepath.c_str(), O_RDWR | O_CREAT, DEFFILEMODE);
			if (file < 0) { throw std::runtime_error(std::strerror(errno)); }
			if (::close(file)) { throw std::runtime_error(std::strerror(errno)); }
#else
#pragma message ("Platform not supported")
#endif
		}

		static void rm(const std::string& filepath)
		{
#if PLATFORM_LINUX == 1
			if (std::remove(filepath.c_str())) { throw std::runtime_error(std::strerror(errno)); }
#else
#pragma message ("Platform not supported")
#endif
		}

		static bool isdir(const std::string& filepath)
		{
#if PLATFORM_LINUX == 1
			struct stat file_stat;
			if (::stat(filepath.c_str(), &file_stat))
			{
				throw std::runtime_error(std::strerror(errno));
			}
			return S_ISDIR(file_stat.st_mode);
#else
#pragma message ("Platform not supported")
#endif
		}

		static void mkdir(const std::string& filepath)
		{
#if PLATFORM_LINUX == 1
			if (::mkdir(filepath.c_str(), DEFAULT_LINUX_FILEMODE)) { throw std::runtime_error(std::strerror(errno)); }
#else
#pragma message ("Platform not supported")
#endif
		}

		static std::list<std::string> lstdir(const std::string& filepath)
		{
			if (!exists(filepath)) { throw std::invalid_argument("filepath not found"); }
			if (!isdir(filepath)) { throw std::invalid_argument("filepath is not directory"); }

			std::list<std::string> entries;
#if PLATFORM_LINUX == 1
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
#else
#pragma message ("Platform not supported")
#endif
			return entries;
		}

		static void rmdir(const std::string& filepath, bool recursive = false)
		{
			if (recursive)
			{
				const auto list = lstdir(filepath);
				for (const auto& entry : list)
				{
					const std::string entrypath = filepath + SEPARATOR + entry;
					if (isdir(entrypath)) { rmdir(entrypath, true); }
					else rm(entrypath);
				}
				rmdir(filepath, false);
			}
			else
			{
#if PLATFORM_LINUX == 1
				if (::rmdir(filepath.c_str())) { throw std::runtime_error(std::strerror(errno)); }
#else
#pragma message ("Platform not supported")
#endif
			}
		}

		static std::string get_temp_dir()
		{

#if PLATFORM_LINUX == 1
			const char* filepath = nullptr;
			if ((filepath = ::getenv("TMPDIR"))) { return filepath; }
			if ((filepath = ::getenv("TEMPDIR"))) { return filepath; }
			if ((filepath = ::getenv("TMP"))) { return filepath; }
			if ((filepath = ::getenv("TEMP"))) { return filepath; }
			return "/tmp";
#else
#pragma message ("Platform not supported")
#endif
		}
	};
}