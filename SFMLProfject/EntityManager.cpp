#include "EntityManager.h"
#include "SystemManager.h"
#include <fstream>
void EntityManager::addComponent(int entityId, ComponentType type)
{
	auto& entity = mEntities[entityId];
	entity.flag |= (1 << (int)type);
	entity.components.push_back(componentFactory[type]());
	systemManager->entityModified(entityId, entity.flag);
}

void EntityManager::addComponentFromStream(int entityId, ComponentType type, std::stringstream& is)
{
	auto& entity = mEntities[entityId];
	entity.flag |= (1 << (int)type);
	CBase* component = componentFactory[type]();
	is >> component;
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

int EntityManager::addEntityFromFile(const std::string& file)
{
	int id = mIdCounter++;
	mEntities[id] = Entity();

	std::ifstream ifs(file);
	while (!ifs.eof())
	{
		std::string type;
		ifs >> type;
		std::string line;
		std::getline(ifs, line);

		std::stringstream ss(line);
		if (type == "POSITION")
		{
			addComponentFromStream(id, ComponentType::Position, ss);
		}
		else if (type == "SPRITESHEET")
		{
			addComponentFromStream(id, ComponentType::SpriteSheet, ss);
		}
		else if (type == "SPRITE")
		{
			addComponentFromStream(id, ComponentType::Sprite, ss);
		}
		else
		{
			throw std::runtime_error("SOMETHING WENT WRONG WITH LOADING THE ENTITY " + std::to_string(id));
		}
	}
	systemManager->entityModified(id, mEntities[id].flag);
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

void EntityManager::purgeEntities()
{
	for (auto& a : mEntities)
	{
		removeEntity(a.first);
	}
}