#pragma once
#include "CBase.h"

enum class EntityState
{
	None,
	Jumping,
	Attacking,
	Dead
};

class CState : public CBase
{
	CState() :
		CBase(ComponentType::State)
	{
		state = EntityState::None;
	}
	EntityState getState() const { return state; }
	void setState(const EntityState& stat) { state = stat; }
private:
	void readIn(std::stringstream& is) override
	{
		int stateInt;
		is >> stateInt;
		state = EntityState(stateInt);
	}
	EntityState state;
};