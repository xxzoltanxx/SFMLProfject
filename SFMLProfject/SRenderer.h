#pragma once
#include <SFML/Graphics.hpp>
#include "SBase.h"

class SRenderer : public SBase
{
public:
	SRenderer(SystemManager* mgr, System sysId);
	void draw(sf::RenderTarget& target);
	void update(float dt) override;
	void handleEvent(unsigned int entityId, SystemEvent event) override;
};