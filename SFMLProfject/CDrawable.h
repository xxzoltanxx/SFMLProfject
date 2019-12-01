#pragma once

#include "CBase.h"
#include "SpriteSheet.h"
#include "TExtureManager.h"

class CDrawable : public CBase
{
public:
	using CBase::CBase;
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void setPosition(const sf::Vector2f& pos) = 0;
};

class CSprite : public CDrawable
{
public:
	CSprite()
		:CDrawable(ComponentType::Sprite)
	{

	}
	void setPosition(const sf::Vector2f& pos) override
	{
		sprite.setPosition(pos);
	}
	void draw(sf::RenderTarget& target) override
	{
		target.draw(sprite);
	}
private:
	void readIn(std::stringstream& is) override
	{
		std::string res;
		is >> res;
		sprite.setTexture(*TextureManager::get()->request(res));
	}
	sf::Sprite sprite;
};
class CSpriteSheet : public CDrawable
{
public:
	CSpriteSheet()
		:CDrawable(ComponentType::SpriteSheet), sheet(nullptr)
	{
	}
	~CSpriteSheet()
	{
		if (sheet) delete sheet;
	}

	void playAnimation(const std::string& animation, bool looping)
	{
		sheet->playAnimation(animation, looping);
	}
	void draw(sf::RenderTarget& target) override
	{
		sheet->draw(target);
	}
	void pause()
	{
		sheet->pause();
	}
	void play()
	{
		sheet->play();
	}
	void setPosition(const sf::Vector2f& pos) override
	{
		sheet->setPosition(pos);
	}
	void update(float dt)
	{
		sheet->update(dt);
	}

private:
	void readIn(std::stringstream& is) override
	{
		std::string sheetPath;
		is >> sheetPath;
		sheet = new SpriteSheet(sheetPath);
	}
	SpriteSheet* sheet;
};