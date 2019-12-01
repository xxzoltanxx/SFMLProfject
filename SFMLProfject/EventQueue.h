#pragma once
#include <queue>

enum class SystemEvent
{
	Spawned = 0,
	EntityModified = 1
};

class EventQueue
{
public:
	void addEvent(SystemEvent event);
	bool pollEvent(SystemEvent& event);
	void clear();
private:
	std::queue<SystemEvent> mEvents;
};