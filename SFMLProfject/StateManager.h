#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

#include "State.h"

class EventHandler;
class SystemManager;
class EntityManager;

struct Context
{
	Context(sf::RenderWindow& win, EventHandler* handler, SystemManager* sysMgr, EntityManager* entMgr)
		: window(win), eventHandler(handler), systemManager(sysMgr), entityManager(entMgr)
	{
	}

	sf::RenderWindow& window;
	EventHandler* eventHandler;
	SystemManager* systemManager;
	EntityManager* entityManager;
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