#ifndef asteroids_common_h
#define	asteroids_common_h

#include "config.h"

#include <string>
#include "filesystem.h"

namespace
{
const std::string DB_NAME = "asteroids.sqlite";
const std::string JSON_NAME = "asteroids.json";
const Path ROOT_PATH = Path(USERS_PATH) / "miclomba" / "downloads";
const Path SERIALIZATION_PATH = ROOT_PATH / JSON_NAME;

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