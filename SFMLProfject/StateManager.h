#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

#include "State.h"

class EventHandler;

struct Context
{
	Context(sf::RenderWindow& win, EventHandler* handler)
		: window(win), eventHandler(handler)
	{
	}

	sf::RenderWindow& window;
	EventHandler* eventHandler;
};

class StateManager
{
public:
	void addState(State state);
	void removeState(State state);

	void draw();
	void update(float dt);

	Context sharedContext;
private:
	template <typename T> void registerState(State state)
	{
		stateFactory[state] = []()-> BaseState * { return new T(); };
	}
	std::vector<BaseState*> states;
	std::unordered_map<State, std::function<BaseState* ()>> stateFactory;
};