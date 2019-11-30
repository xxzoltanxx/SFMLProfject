#include "EventHandler.h"

void EventHandler::update()
{
	for (auto& a : mBindings)
	{
		auto& events = a.second->mEvents;
		for (auto& eventBinding : events)
		{
			if (eventBinding.first == RegisteredEvent::Keyboard)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key((int)eventBinding.second.keyCode)))
				{
					++a.second->count;
					a.second->info.lastKeyCode = eventBinding.second.keyCode;
				}
			}
			else if (eventBinding.first == RegisteredEvent::Mouse)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button((int)eventBinding.second.keyCode)))
				{
					++a.second->count;
					a.second->info.lastKeyCode = eventBinding.second.keyCode;
				}
			}
		}
		if (a.second->count == a.second->mEvents.size())
		{
			mCallbacks[a.second->name](a.second->info);
		}
		a.second->count = 0;
	}
}
void EventHandler::handleEvent(sf::Event& event)
{
	for (auto& a : mBindings)
	{
		auto& events = a.second->mEvents;
		for (auto& eventBinding : events)
		{
			if ((int)eventBinding.first == (int)event.type)
			{
				if (eventBinding.first == RegisteredEvent::KeyPress || eventBinding.first == RegisteredEvent::KeyRelease)
				{
					if (eventBinding.second.keyCode == event.key.code)
					{
						++a.second->count;
						a.second->info.lastKeyCode = eventBinding.second.keyCode;
					}
				}
				else if (eventBinding.first == RegisteredEvent::MousePressed || eventBinding.first == RegisteredEvent::MouseReleased)
				{
					if (eventBinding.second.keyCode == event.mouseButton.button)
					{
						++a.second->count;
						a.second->info.mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
					}
				}
				else
				{
					++a.second->count;
				}
			}
		}
	}
}