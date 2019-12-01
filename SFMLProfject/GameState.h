#pragma once
#include "State.h"
class GameState : public BaseState
{
public:
	GameState(StateManager* mgr);

	void draw(sf::RenderTarget& target) override;
	void update(float dt) override;

	void onCreate() override;
	void onDestroy() override;
};