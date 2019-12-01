#pragma once
#include <SFML/Graphics.hpp>

#include "MessageHandler.h"
#include "EventQueue.h"
#include "SBase.h"

class EntityManager;
class SystemManager
{
public:
	void setEntityManager(EntityManager* mgr)
	{
		entityManager = mgr;
	}

	void update(float dt);
	void handleEvents();
	void draw(sf::RenderTarget& target);
	void addEvent(SystemEvent event, const unsigned int& entityId);
	void removeEntity(const unsigned int& entityId);
	void entityModified(const unsigned int& entityId, const unsigned int& entityMask);

	template <typename T> T* getSystem(System system)
	{
		for (auto& system : systems)
		{
			if (system.second->getId() == system)
			{
				return static_cast<T*>(system.second);
			}
		}
	}

	MessageHandler messageHandler;
private:
	std::unordered_map<System, SBase*> systems;
	std::unordered_map<unsigned int, EventQueue> entityEventQueues;
	EntityManager* entityManager;
};