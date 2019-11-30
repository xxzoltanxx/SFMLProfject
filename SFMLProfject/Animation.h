#pragma once
#include <SFML/Graphics.hpp>

class SpriteSheet;

class Animation
{
public:
	Animation(std::string texture, unsigned short frameCount, float frameStep, SpriteSheet* sheet, sf::Vector2i cellSize);
	~Animation();
	void update(float dt);
	void reset(bool looping);
	void cropSprite();
private:
	sf::Texture* boundTexture = nullptr;
	unsigned short currentFrame = 0;
	unsigned short framesNum = 0;
	float frameStep;
	float timer = 0;
	sf::Vector2i cellSize;
	sf::IntRect bounds;
	bool isLooping = false;

	SpriteSheet* sheet = nullptr;
};