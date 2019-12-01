#include "EventQueue.h"

void EventQueue::addEvent(SystemEvent event)
{
	mEvents.emplace(event);
}

bool EventQueue::pollEvent(SystemEvent& event)
{
	if (mEvents.empty()) return false;
	event = mEvents.front();
	mEvents.pop();
	return true;
}

void EventQueue::clear()
{
	while (!mEvents.empty())
	{
		mEvents.pop();
	}
}