#pragma once
#include "ResourceManager.h"
#include "SFML/Graphics.hpp"
class FontManager : public ResourceManager<FontManager, sf::Font>
{
public:
	static FontManager* get()
	{
		if (!instance)
		{
			instance = new FontManager();
		}
		return instance;
	}
	sf::Font* load(std::string resource)
	{
		sf::Font* font = new sf::Font();
		font->loadFromFile(paths[resource]);
		return font;
	}
	void setupPaths(const std::string& file);
private:
	FontManager() : ResourceManager() { setupPaths("fonts.cfg"); }
	static FontManager* instance;
};