#include "State.h"

BaseState::BaseState(StateManager* mgr, bool isTranscendent, bool isTranslucent, State id)
{
	this->stateId = id;
	this->isTranscendent = isTranscendent;
	this->isTranslucent = isTranslucent;
	this->stateManager = mgr;
}