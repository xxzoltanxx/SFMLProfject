#include "AudioManager.h"

#include "Utilities.h"

AudioManager* AudioManager::instance = nullptr;

void AudioManager::setupPaths(const std::string& file)
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