#include "FontManager.h"

#include "Utilities.h"

FontManager* FontManager::instance = nullptr;

void FontManager::setupPaths(const std::string& file)
{
	std::string workingDirectory = Utils::GetWorkingDirectory();
	std::ifstream ifs(workingDirectory + file);
	std::string resource;
	std::string path;
	while (ifs >> resource >> path)
	{
		paths[resource] = workingDirectory + path;
	}
}