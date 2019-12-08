#include <fstream>
#include <sstream>

#include "EventHandler.h"
#include "Utilities.h"

EventHandler::~EventHandler()
{
	for (auto& eventBinding : mBindings)
	{
		for (auto& eventData : eventBinding.second->mEvents)
		{
			if (eventData.first == RegisteredEvent::GUIClick || eventData.first == RegisteredEvent::GUIEnter || eventData.first == RegisteredEvent::GUIExit || eventData.first == RegisteredEvent::GUIRelease)
			{
				delete[] eventData.second.guiEvent.element;
				delete[] eventData.second.guiEvent.interfaceC;
			}
		}
	}
}

EventHandler::EventHandler(const std::string& keysConfig)
{
	currentState = State::GameState;
	std::string path = Utils::GetWorkingDirectory() + keysConfig;
	std::ifstream ifs(path);
	std::string line;
	while (std::getline(ifs, line))
	{
		std::stringstream ss(line);
		std::string bindingName;
		ss >> bindingName;
		std::string separated;
		mBindings[bindingName] = new Binding();
		while (ss >> separated)
		{
			auto delimiter = separated.find(':');
			int firstVal = std::stoi(separated.substr(0, delimiter));
			if (firstVal == (int)RegisteredEvent::GUIClick || firstVal == (int)RegisteredEvent::GUIEnter || firstVal == (int)RegisteredEvent::GUIExit || firstVal == (int)RegisteredEvent::GUIRelease)
			{
				auto secondDelimiter = separated.find(':', delimiter + 1);
				std::string secondValue = separated.substr(delimiter + 1, secondDelimiter - delimiter);
				std::string thirdValue = separated.substr(secondDelimiter + 1, separated.length() - secondDelimiter);
				EventData data = EventData();
				char* copyElement = new char[secondValue.length() + 1];
				char* copyInterface = new char[secondValue.length() + 1];
				strcpy_s(copyElement, secondValue.length(), secondValue.c_str());
				strcpy_s(copyInterface, thirdValue.length(), thirdValue.c_str());
				data.guiEvent.type = GUIEventType((firstVal - int(RegisteredEvent::GUIRelease)));
				data.guiEvent.element = copyElement;
				data.guiEvent.interfaceC = copyInterface;
				mBindings[bindingName]->mEvents.push_back(std::make_pair(RegisteredEvent(firstVal), data));
			}
			else
			{
				int secondVal = std::stoi(separated.substr(delimiter + 1, separated.length() - delimiter));
				EventData data = EventData();
				data.keyCode = secondVal;
				mBindings[bindingName]->mEvents.push_back(std::make_pair(RegisteredEvent(firstVal), data));
			}
		}
	}
}

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
			mCallbacks[getCurrentState()][a.second->name](a.second->info);
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
				++a.second->count;
			}
		}
	}
}

void EventHandler::handleEvent(GUIEvent& event)
{
	for (auto& a : mBindings)
	{
		auto& events = a.second->mEvents;
		for (auto& eventBinding : events)
		{
			if ((int)eventBinding.first == (int)event.type + (int)RegisteredEvent::GUIRelease)
			{
				if (strcmp(event.interfaceC, eventBinding.second.guiEvent.interfaceC) == 0 && strcmp(event.element, eventBinding.second.guiEvent.element) == 0)
				{
					++a.second->count;
				}
			}
		}
	}
}