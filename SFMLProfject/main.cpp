#include <unordered_map>
#include <vector>
#include <functional>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "CBase.h"
int main()
{
	return 0;
}

enum class ObserverMessage
{

};
struct ObserverPayload
{
	ObserverMessage type;
	int mReceiver;

	bool mBool;
	float mX, mY;
	int mInt;
};

class Observer
{
public:
	virtual void notify(ObserverPayload payload) = 0;
};