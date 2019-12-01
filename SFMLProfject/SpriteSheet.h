#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "CBase.h"

class Animation;

class SpriteSheet
{
public:
	SpriteSheet(const std::string& sheetFile)
	{
		readIn(sheetFile);
	}
	void cropSprite(sf::Texture* texture, const sf::IntRect& textureRect);
	void update(float dt);
	void playAnimation(const std::string& animation, bool looping);
	void draw(sf::RenderTarget& target);
	void pause();
	void play();
	void setPosition(const sf::Vector2f& pos)
	{
		sprite.setPosition(pos);
	}
	~SpriteSheet();
private:
	void readIn(const std::string& path);
	sf::Sprite sprite;
	std::unordered_map<std::string, Animation*> mAnimations;
	Animation* currentAnimation = nullptr;

	bool isPlaying = true;
};