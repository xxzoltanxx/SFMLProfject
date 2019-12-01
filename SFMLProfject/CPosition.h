#pragma once
#include <SFML/Graphics.hpp>

#include "CBase.h"

class CPosition : public CBase
{
public:
	CPosition()
		:CBase(ComponentType::Position)
	{

	}
	sf::Vector2f getPosition() const { return pos; }
	void setPosition(const sf::Vector2f& posi) { pos = posi; }
	void readIn(std::stringstream& is) override
	{
		is >> pos.x;
		is >> pos.y;
	}
private:
	sf::Vector2f pos;
};