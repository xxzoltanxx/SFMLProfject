#pragma once
#include "Observer.h"
#include "EventQueue.h"
#include "CBase.h"
class SystemManager;
enum class System
{
	Renderer = 0
};

class SBase : public Observer
{
public:
	SBase(SystemManager* mgr, System sysId);
	bool matchesFlag(const unsigned int& req) const;
	void addEntity(const unsigned int& entityId);
	bool hasEntity(const unsigned int& entityId);
	void removeEntity(const unsigned int& entityId);
	virtual void handleEvent(unsigned int entityId, SystemEvent event) = 0;
	virtual void update(float dt) = 0;
private:
	System systemId;
	std::vector<unsigned int> mEntities;
	std::vector<unsigned int> requirementFlags;
	SystemManager* manager;
};