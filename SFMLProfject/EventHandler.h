#pragma once
#include "StateManager.h"
enum class RegisteredEvent
{
	KeyPress = sf::Event::KeyPressed,
	KeyRelease = sf::Event::KeyReleased,
	MousePressed = sf::Event::MouseButtonPressed,
	MouseReleased = sf::Event::MouseButtonReleased,
	Keyboard = sf::Event::Count + 1,
	Mouse
};

class EventData
{
public:
	union
	{
		int keyCode = 0;
	};
};

using Callbacks = std::unordered_map<State, std::unordered_map<std::string, std::function<void(EventInfo& info)>>>;

struct EventInfo
{
	int lastKeyCode;
	sf::Vector2i mousePosition;
	std::string name;
};

class Binding
{
public:
	std::string name;
	EventInfo info;
	std::vector<std::pair<RegisteredEvent, EventData>> mEvents;
	int count = 0;
};
class EventHandler
{
public:
	void update();
	void handleEvent(sf::Event& event);
	template <typename T> void registerCallback(State state, std::string bind,void (T::* func)(EventInfo& info), T* instance)
	{
		if (mCallbacks.find(state) == mCallbacks.end())
		{
			mCallbacks[state] = std::unordered_map<std::string, std::function<void(EventInfo & info)>>();
		}
		mCallbacks[state].emplace(bind, std::bind(func, instance, std::placeholders::_1));
	}
private:
	std::unordered_map<std::string, Binding*> mBindings;
	Callbacks mCallbacks;
};