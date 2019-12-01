#include "SystemManager.h"
#include "SRenderer.h"

SystemManager::SystemManager()
{
	systems[System::Renderer] = new SRenderer(this);
}

SystemManager::~SystemManager()
{
	purgeSystems();
}

void SystemManager::purgeSystems()
{
	for (auto& a : systems)
	{
		delete a.second;
	}
}

void SystemManager::handleEvents()
{
	SystemEvent event;
	for (auto& a : entityEventQueues)
	{
		while (a.second.pollEvent(event))
		{
			for (auto& system : systems)
			{
				if (system.second->hasEntity(a.first))
				{
					system.second->handleEvent(a.first, event);
				}
			}
		}
	}
}
void SystemManager::entityModified(const unsigned int& entityId, const unsigned int& entityMask)
{
	for (auto& a : systems)
	{
		if (!a.second->hasEntity(entityId) && a.second->matchesFlag(entityMask))
		{
			a.second->addEntity(entityId);
		}
		else if (a.second->hasEntity(entityId) && !a.second->matchesFlag(entityMask))
		{
			a.second->removeEntity(entityId);
		}
	}
}

void SystemManager::update(float dt)
{
	for (auto& a : systems)
	{
		a.second->update(dt);
	}
}

void SystemManager::addEvent(SystemEvent event, const unsigned int& entityId)
{
	if (entityEventQueues.find(entityId) == entityEventQueues.end())
	{
		entityEventQueues[entityId] = EventQueue();
	}
	entityEventQueues[entityId].addEvent(event);
}

void SystemManager::removeEntity(const unsigned int& entityId)
{
	for (auto& a : systems)
	{
		a.second->removeEntity(entityId);
	}
	auto entityEvents = entityEventQueues.find(entityId);
	if (entityEvents != entityEventQueues.end())
	{
		entityEventQueues.erase(entityEvents);
	}
}

void SystemManager::draw(sf::RenderTarget& target)
{
	//TODO once SRenderer is set up
}