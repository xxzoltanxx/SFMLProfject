#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class StateManager;

enum class State
{
	GameState = 0
};
class BaseState
{
public:
	State stateId;
	BaseState(StateManager* mgr, bool isTranscendent, bool isTranslucent, State id);

	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void update(float dt) = 0;

	bool getIsTranslucent() const { return isTranslucent; }
	bool getIsTranscendent() const { return isTranscendent; }
protected:
	bool isTranscendent = false;
	bool isTranslucent = false;
	StateManager* stateManager;
};