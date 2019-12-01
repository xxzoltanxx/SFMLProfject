#pragma once
#include "ResourceManager.h"
#include "SFML/Graphics.hpp"
class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	static TextureManager* get()
	{
		if (!instance)
		{
			instance = new TextureManager();
		}
		return instance;
	}
	sf::Texture* load(std::string resource)
	{
		sf::Texture* text = new sf::Texture();
		text->loadFromFile(paths[resource]);
		return text;
	}
	void setupPaths(const std::string& file);
private:
	TextureManager() : ResourceManager() { setupPaths("textures.cfg"); }
	static TextureManager* instance;
};