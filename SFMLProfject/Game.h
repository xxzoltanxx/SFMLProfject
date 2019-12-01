#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "SystemManager.h"
#include "EntityManager.h"
#include "EventHandler.h"
#include "StateManager.h"

class Game
{
public:
	Game();
	void run();
private:
	void update(float dt);
	void draw();
	void handleEvents(sf::Event& event);

	EntityManager* entityManager;
	SystemManager* systemManager;
	EventHandler* eventHandler;
	StateManager* stateManager;
	sf::RenderWindow* window;

	sf::Clock timer;
	sf::Time elapsedTime = sf::seconds(0);
	sf::Time frameTime = sf::seconds(1.0f / 60.0f);
};