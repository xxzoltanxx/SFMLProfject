#include "Game.h"
#include "TextureManager.h"
#include "FontManager.h"
Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(640, 640, 32), "TEST WINDOW");
	systemManager = new SystemManager();
	entityManager = new EntityManager(systemManager);
	systemManager->setEntityManager(entityManager);
	eventHandler = new EventHandler("keys.cfg");
	Context context(*window, eventHandler, systemManager, entityManager);
	guiManager = new GUIManager(context, eventHandler);
	context.guiManager = guiManager;
	stateManager = new StateManager(context);

	GUIInterface* newInterface = new GUIInterface(guiManager, "TEST");
	newInterface->addButton("TEST", "TEST");
	guiManager->addInterface(State::GameState, newInterface, "TEST");
	//create textureManager instance so the config loads
	TextureManager::get();
	FontManager::get();
	stateManager->addState(State::GameState);
}

void Game::handleEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		eventHandler->handleEvent(event);
	}
	GUIEvent guiEvent;
	while (guiManager->pollEvent(guiEvent))
	{
		eventHandler->handleEvent(guiEvent);
	}
	eventHandler->update();
}

void Game::update(float dt)
{
	stateManager->update(dt);
	guiManager->update(dt);
}

void Game::draw()
{
	stateManager->draw();
	guiManager->draw(*window);
}

void Game::run()
{
	while (window->isOpen())
	{
		handleEvents();
		elapsedTime += timer.restart();

		while (elapsedTime > frameTime)
		{
			update(frameTime.asSeconds());
			elapsedTime -= frameTime;
		}
		window->clear();
		draw();
		window->display();
	}
}