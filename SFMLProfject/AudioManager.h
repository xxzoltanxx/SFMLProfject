#pragma once
#include "ResourceManager.h"
#include <SFML/Audio.hpp>"

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer>
{
public:
	static AudioManager* get()
	{
		if (!instance)
		{
			instance = new AudioManager();
		}
		return instance;
	}
	sf::SoundBuffer* load(std::string resource)
	{
		sf::SoundBuffer* buffer = new sf::SoundBuffer();
		buffer->loadFromFile(paths[resource]);
		return buffer;
	}
	void setupPaths(const std::string& file);
private:
	AudioManager() : ResourceManager() { setupPaths("sounds.cfg"); }
	static AudioManager* instance;
};