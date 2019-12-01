#include "Game.h"
#include "TextureManager.h"
Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(640, 640, 32), "TEST WINDOW");
	systemManager = new SystemManager();
	entityManager = new EntityManager(systemManager);
	systemManager->setEntityManager(entityManager);
	eventHandler = new EventHandler("keys.cfg");
	Context context(*window, eventHandler, systemManager, entityManager);
	stateManager = new StateManager(context);

	//create textureManager instance so the config loads
	TextureManager::get();
	stateManager->addState(State::GameState);
}

void Game::handleEvents(sf::Event& event)
{
	eventHandler->handleEvent(event);
	eventHandler->update();
}

void Game::update(float dt)
{
	stateManager->update(dt);
}

void Game::draw()
{
	stateManager->draw();
}

void Game::run()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			eventHandler->handleEvent(event);
		}
		elapsedTime += timer.restart();

		while (elapsedTime > frameTime)
		{
			update(frameTime.asSeconds());
			elapsedTime -= frameTime;
		}
		draw();
	}
}