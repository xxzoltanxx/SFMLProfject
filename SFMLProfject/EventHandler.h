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
	template <typename T> void registerCallback(std::string bind,void (T::* func)(EventInfo info), T* instance)
	{
		mCallbacks.emplace(bind, std::bind(func, instance, std::placeholders::_1));
	}
private:
	std::unordered_map<std::string, Binding*> mBindings;
	std::unordered_map<std::string, std::function<void(EventInfo& info)>> mCallbacks;
};