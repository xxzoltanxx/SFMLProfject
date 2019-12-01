#include "SpriteSheet.h"
#include "Animation.h"
#include <fstream>
#include <sstream>

void SpriteSheet::draw(sf::RenderTarget& target)
{
	target.draw(sprite);
}

void SpriteSheet::play()
{
	isPlaying = true;
}

void SpriteSheet::pause()
{
	isPlaying = false;
}

void SpriteSheet::cropSprite(sf::Texture* texture, const sf::IntRect& textureRect)
{
	sprite.setTexture(*texture);
	sprite.setTextureRect(textureRect);
}

void SpriteSheet::update(float dt)
{
	if (currentAnimation && isPlaying)
	{
		currentAnimation->update(dt);
	}
}

void SpriteSheet::playAnimation(const std::string& animation, bool looping)
{
	if (mAnimations.find(animation) != mAnimations.end())
	{
		if (currentAnimation != mAnimations[animation])
		{
			currentAnimation = mAnimations[animation];
			currentAnimation->reset(looping);
		}
	}
	play();
}

SpriteSheet::~SpriteSheet()
{
	for (auto& a : mAnimations)
	{
		delete a.second;
	}
}

void SpriteSheet::readIn(const std::string& path)
{
	std::ifstream is(path);
	std::string line;
	while (std::getline(is, line))
	{
		std::stringstream ss(line);
		std::string animationName;
		std::string texture;
		int frameNum;
		float frameStep;
		int cellSizeX;
		int cellSizeY;

		ss >> animationName >> texture >> frameNum >> frameStep >> cellSizeX >> cellSizeY;
		Animation* toInsert = new Animation(texture, (unsigned short) frameNum, frameStep, this, sf::Vector2i(cellSizeX, cellSizeY));
		mAnimations[animationName] = toInsert;
	}
}
