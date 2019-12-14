#include "GameState.h"
#include "StateManager.h"
#include "SystemManager.h"
#include "EntityManager.h"

GameState::GameState(StateManager* mgr)
	:BaseState(mgr, false, false, State::GameState)
{

}

void GameState::draw(sf::RenderTarget& target)
{
	mGameMap.draw(target);
	stateManager->sharedContext.systemManager->draw(target);
}

void GameState::update(float dt)
{
	stateManager->sharedContext.systemManager->update(dt);
}

void GameState::onCreate()
{
	stateManager->sharedContext.entityManager->addEntityFromFile("TestEntity.txt");
	mGameMap.loadMap("testMap.xml");
}

void GameState::onDestroy()
{

}