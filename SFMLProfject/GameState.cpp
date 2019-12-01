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
	stateManager->sharedContext.systemManager->draw(target);
}

void GameState::update(float dt)
{
	stateManager->sharedContext.systemManager->update(dt);
}

void GameState::onCreate()
{
	stateManager->sharedContext.entityManager->addEntityFromFile("TestEntity.txt");
}

void GameState::onDestroy()
{

}