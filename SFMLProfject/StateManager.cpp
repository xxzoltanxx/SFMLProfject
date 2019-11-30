#include "StateManager.h"
void StateManager::draw()
{
	for (int i = states.size() - 1; i >= 0; --i)
	{
		states[i]->draw(sharedContext.window);
		if (!states[i]->getIsTranslucent())
		{
			break;
		}
	}
}

void StateManager::update(float dt)
{
	for (int i = states.size() - 1; i >= 0; --i)
	{
		states[i]->update(dt);
		if (!states[i]->getIsTranscendent())
		{
			break;
		}
	}
}
void StateManager::addState(State state)
{
	states.push_back(stateFactory[state]());
}

void StateManager::removeState(State state)
{
	auto stateIter = std::find_if(states.begin(), states.end(), [state](BaseState* stateIter)->bool { return state == stateIter->stateId; });
	if (stateIter != states.end())
	{
		delete (*stateIter);
		states.erase(stateIter);
	}
}