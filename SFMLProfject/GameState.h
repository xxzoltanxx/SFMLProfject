#pragma once
#include "State.h"
#include "Map.h"
class GameState : public BaseState
{
public:
	GameState(StateManager* mgr);

	void draw(sf::RenderTarget& target) override;
	void update(float dt) override;

	void onCreate() override;
	void onDestroy() override;

private:
	Map mGameMap;
};