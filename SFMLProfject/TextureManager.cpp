#include "TextureManager.h"

#include "Utilities.h"

TextureManager* TextureManager::instance = nullptr;

void TextureManager::setupPaths(const std::string& file)
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