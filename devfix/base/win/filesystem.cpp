//
// Created by core on 6/27/20.
//

#include "../platform.h"

#if PLATFORM_WINDOWS == 1

#include "../filesystem.h"

namespace devfix::base
{
	const char filesystem::SEPARATOR = '\\';


	bool filesystem::exists(const std::string& filepath)
	{
		throw std::runtime_error("not implemented");
	}

	bool filesystem::isfile(const std::string& filepath)
	{
		throw std::runtime_error("not implemented");
	}

	void filesystem::touch(const std::string& filepath)
	{
		throw std::runtime_error("not implemented");
	}

	void filesystem::rm(const std::string& filepath)
	{
		throw std::runtime_error("not implemented");
	}

	bool filesystem::isdir(const std::string& filepath)
	{
		throw std::runtime_error("not implemented");
	}

	void filesystem::mkdir(const std::string& filepath)
	{
		throw std::runtime_error("not implemented");
	}

	std::list<std::string> filesystem::lstdir(const std::string& filepath)
	{
		throw std::runtime_error("not implemented");
	}

	void filesystem::rmdir(const std::string& filepath, bool recursive)
	{
		throw std::runtime_error("not implemented");
	}

	std::string filesystem::get_temp_dir()
	{
		throw std::runtime_error("not implemented");
	}
}

#endif
