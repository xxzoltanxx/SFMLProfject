#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "CBase.h"

class Animation;

class SpriteSheet
{
public:
	void cropSprite(sf::Texture* texture, const sf::IntRect& textureRect);
	void update(float dt);
	void playAnimation(const std::string& animation, bool looping);
	void draw(sf::RenderTarget& target);
	void pause();
	void play();

	~SpriteSheet();
private:
	void readIn(std::stringstream& is);

	sf::Sprite sprite;
	std::unordered_map<std::string, Animation*> mAnimations;
	Animation* currentAnimation = nullptr;

	bool isPlaying = true;
};