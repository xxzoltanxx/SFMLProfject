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

class EntityManager
{
public:
	int addEntity(int flag);
	void removeEntity(int id);

	void addComponent(int entityId, ComponentType type);
	void removeComponent(int entityId, ComponentType type);

	template <typename T> T* getComponent(int entityId, ComponentType component)
	{
		auto& entity = mEntities[entityId];
		auto Component = std::find_if(entity.components.begin(), entity.components.end(), [component](CBase* item)-> bool {return item->getType = component; });
		return static_cast<T*>(*Component);
	}
private:
	template <typename T> void registerComponent(ComponentType type)
	{
		componentFactory[type] = []()->CBase * {return new T(); };
	}
	std::unordered_map<int, Entity> mEntities;
	std::unordered_map<ComponentType, std::function<CBase* ()>> componentFactory;
	int mIdCounter = 0;
};