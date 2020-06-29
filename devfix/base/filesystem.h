//
// Created by core on 6/27/20.
//

#pragma once

#include <string>
#include <list>

namespace devfix::base
{
	struct filesystem
	{
		const static char SEPARATOR;
		const static std::string_view SEPARATOR_SV;

		[[nodiscard]] static bool exists(const std::string& filepath);
		[[nodiscard]] static bool is_abs_path(const std::string& filepath);
		[[nodiscard]] static bool isfile(const std::string& filepath);
		static void touch(const std::string& filepath);
		static void rm(const std::string& filepath);
		[[nodiscard]] static bool isdir(const std::string& filepath);
		static void mkdir(const std::string& filepath, bool parents = false);
		[[nodiscard]] static std::string basename(const std::string& filepath);
		[[nodiscard]] static std::string dirname(const std::string& filepath);
		[[nodiscard]] static std::list<std::string> lstdir(const std::string& filepath);
		static void rmdir(const std::string& filepath, bool recursive = false);
		[[nodiscard]] static std::string get_temp_dir();
	};
}
