#pragma once
#include <SFML/Graphics.hpp>
#include "SBase.h"

class SRenderer : public SBase
{
public:
	SRenderer(SystemManager* mgr);
	void draw(sf::RenderTarget& target);
	void update(float dt) override;
	void handleEvent(unsigned int entityId, SystemEvent event) override;
	void notify(const ObserverPayload& payload);
};