#include "EntityManager.h"
#include "SystemManager.h"
void EntityManager::addComponent(int entityId, ComponentType type)
{
	auto& entity = mEntities[entityId];
	entity.flag |= (1 << (int)type);
	entity.components.push_back(componentFactory[type]());
	systemManager->entityModified(entityId, entity.flag);
}

void EntityManager::removeComponent(int entityId, ComponentType type)
{
	auto& entity = mEntities[entityId];
	entity.flag &= ~(1 << (int)type);
	auto Component = std::find_if(entity.components.begin(), entity.components.end(), [type](CBase* item)-> bool {return item->getType() == type; });
	delete (*Component);
	entity.components.erase(Component);
	systemManager->entityModified(entityId, entity.flag);
}

int EntityManager::addEntity(int flag)
{
	int id = mIdCounter++;
	mEntities[id] = Entity();
	for (int i = 0; i < componentTypes; ++i)
	{
		if (flag & (1 << i))
		{
			addComponent(id, ComponentType(i));
		}
	}
	systemManager->entityModified(id, flag);
	return id;
}

void EntityManager::removeEntity(int id)
{
	auto iter = mEntities.find(id);
	for (int i = 0; i < iter->second.components.size(); ++i)
	{
		delete iter->second.components[i];
	}
	systemManager->removeEntity(id);
	mEntities.erase(iter);
}