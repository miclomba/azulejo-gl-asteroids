#ifndef asteroids_common_h
#define	asteroids_common_h

#include "config.h"

#include <filesystem>
#include <string>

namespace
{
const std::string DB_NAME = "asteroids.sqlite";
const std::string JSON_NAME = "asteroids.json";
const std::filesystem::path ROOT_PATH = std::filesystem::path(USERS_PATH) / "miclomba" / "desktop";
const std::filesystem::path SERIALIZATION_PATH = ROOT_PATH / JSON_NAME;

std::string FormatKey(const std::string& key)
{
	std::string formattedKey;
	for (char c : key)
	{
		if (c != '-' && c != '_') formattedKey.push_back(c);
	}

	return formattedKey;
}
} // end namespace

#endif // asteroids_common_h