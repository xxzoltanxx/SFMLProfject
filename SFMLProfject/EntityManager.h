#pragma once
#include "CBase.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

struct Entity
{
	int flag = 0;
	std::vector<CBase*> components;
};

class SystemManager;
class EntityManager
{
public:
	EntityManager(SystemManager* manager)
	{
		systemManager = manager;
	}
	int addEntity(int flag);
	int addEntityFromFile(const std::string& file);
	void removeEntity(int id);

	void addComponent(int entityId, ComponentType type);
	void addComponentFromStream(int entityId, ComponentType type, std::stringstream& is);
	void removeComponent(int entityId, ComponentType type);

	template <typename T> T* getComponent(int entityId, ComponentType component)
	{
		auto& entity = mEntities[entityId];
		auto Component = std::find_if(entity.components.begin(), entity.components.end(), [component](CBase* item)-> bool {return item->getType() == component; });
		if (Component == entity.components.end())
		{
			return nullptr;
		}
		return static_cast<T*>(*Component);
	}

	void purgeEntities();
private:
	template <typename T> void registerComponent(ComponentType type)
	{
		componentFactory[type] = []()->CBase * {return new T(); };
	}
	std::unordered_map<int, Entity> mEntities;
	std::unordered_map<ComponentType, std::function<CBase* ()>> componentFactory;
	int mIdCounter = 0;
	SystemManager* systemManager;
};