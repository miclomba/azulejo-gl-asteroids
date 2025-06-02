#ifndef asteroids_common_h
#define asteroids_common_h

#include "config/config.h"

#include <string>
#include "config/filesystem.h"

namespace
{
	const std::string DB_NAME = "asteroids.sqlite";
	const std::string JSON_NAME = "asteroids.json";
#ifdef _WIN32
	const Path ROOT_PATH = Path(std::getenv("USERPROFILE")) / "downloads";
#else
	const Path ROOT_PATH = Path(std::getenv("HOME")) / "downloads";
#endif
	const Path SERIALIZATION_PATH = ROOT_PATH / JSON_NAME;

	std::string FormatKey(const std::string &key)
	{
		std::string formattedKey;
		for (char c : key)
		{
			if (c != '-' && c != '_')
				formattedKey.push_back(c);
		}

		return formattedKey;
	}
} // end namespace

#endif // asteroids_common_h