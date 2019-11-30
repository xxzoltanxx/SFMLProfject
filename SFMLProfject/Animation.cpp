#include "Animation.h"
#include "SpriteSheet.h"
#include "TextureManager.h"

Animation::Animation(std::string texture, unsigned short frameCount, float frameSte, SpriteSheet* shee, sf::Vector2i cellSiz)
	:  framesNum(frameCount), frameStep(frameSte), sheet(shee), cellSize(cellSiz), boundTexture(nullptr)
{
	boundTexture = TextureManager::get()->request(texture);
}

Animation::~Animation()
{
	TextureManager::get()->release(boundTexture);
}

void Animation::reset(bool looping)
{
	currentFrame = 0;
	timer = 0;
	isLooping = looping;
}

void Animation::update(float dt)
{
	timer += dt;
	if (timer >= frameStep)
	{
		if (currentFrame < framesNum - 1)
		{
			currentFrame++;
		}
		else if (currentFrame == framesNum - 1 && isLooping)
		{
			currentFrame = 0;
		}
		timer = 0;
		cropSprite();
	}
}

void Animation::cropSprite()
{
	bounds.left = currentFrame * cellSize.x;
	bounds.top = 0;
	bounds.width = cellSize.x;
	bounds.height = cellSize.y;
	sheet->cropSprite(boundTexture, bounds);
}
