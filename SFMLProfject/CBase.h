#pragma once
#include <sstream>

static constexpr int componentTypes = 5;
enum class ComponentType
{
	Position = 0,
	SpriteSheet = 1,
};

class CBase
{
public:
	CBase(ComponentType typ) :
		type(typ)
	{

	}
	friend std::stringstream& operator>>(std::stringstream& is, CBase* comp)
	{
		comp->readIn(is);
		return is;
	}
	ComponentType getType() const { return type; }
protected:
	virtual void readIn(std::stringstream& is) = 0;
	ComponentType type;
};