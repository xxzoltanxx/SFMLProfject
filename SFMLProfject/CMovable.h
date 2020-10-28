#pragma once
#include "CBase.h"
#include <SFML/Graphics.hpp>

class CMovable : public CBase
{
public:
	CMovable() : CBase(ComponentType::Movable) {}
	sf::Vector2f getAcceleration() const { return mAcceleration; }
	sf::Vector2f getVelocity() const { return mVelocity; }
	sf::Vector2f getTerminalVelocity() const { return mTerminalVelocity; }

	void setVelocity(const sf::Vector2f& vel) { mVelocity = vel; }
	void readIn(std::stringstream& is) override
	{
		is >> mAcceleration.x >> mAcceleration.y >> mVelocity.x >> mVelocity.y >> mTerminalVelocity.x >> mTerminalVelocity.y;
	}
private:
	sf::Vector2f mAcceleration;
	sf::Vector2f mVelocity;
	sf::Vector2f mTerminalVelocity;
};