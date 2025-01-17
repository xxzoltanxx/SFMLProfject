#include "StateManager.h"
#include "EventHandler.h"
#include "GameState.h"

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
	sharedContext.eventHandler->setCurrentState(state);
	states.back()->onCreate();
}

void StateManager::removeState(State state)
{
	auto stateIter = std::find_if(states.begin(), states.end(), [state](BaseState* stateIter)->bool { return state == stateIter->stateId; });
	if (stateIter != states.end())
	{
		(*stateIter)->onDestroy();
		delete (*stateIter);
		states.erase(stateIter);
	}
	if (states.size() > 0)
		sharedContext.eventHandler->setCurrentState(states.back()->stateId);
}

StateManager::StateManager(Context sContext)
:sharedContext(sContext)
{
	registerState<GameState>(State::GameState);
}