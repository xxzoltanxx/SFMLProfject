#include "GUIManager.h"
#include "EventHandler.h"
#include <fstream>
#include <sstream>

void GUIManager::draw(sf::RenderTarget& target)
{
	for (auto& a : mGuiMappings[mCurrentState])
	{
		if (a.second->isActive)
		{
			if (a.second->mNeedsContentRedraw)
			{
				a.second->redrawContent();
			}
			if (a.second->mNeedsBackdropRedraw)
			{
				a.second->redrawBackdrop();
			}
			if (a.second->mNeedsControlRedraw)
			{
				a.second->redrawControls();
			}
			a.second->draw(target);
		}
	}
}

void GUIManager::addEvent(const GUIEvent& event)
{
	mGuiEvents.emplace(event);
}

bool GUIManager::pollEvent(GUIEvent& event)
{
	if (mGuiEvents.empty())
	{
		return false;
	}
	event = mGuiEvents.front();
	mGuiEvents.pop();
	return true;
}

void GUIManager::setCurrentState(State state)
{
	mCurrentState = state;
}

void GUIManager::addInterface(State state, GUIInterface* interface, std::string name)
{
	mGuiMappings[state].emplace(name, interface);
}

void GUIManager::update(float dt)
{
	sf::Vector2i currentPosition = sf::Mouse::getPosition(sharedContext.window);
	for (auto& a : mGuiMappings[mCurrentState])
	{
		a.second->update(dt);
		if (a.second->isWithin(currentPosition) && a.second->isActive)
		{
			if (a.second->mCurrentState == GUIState::Normal)
			{
				a.second->onHover(currentPosition);
			}
		}
		else if (!a.second->isWithin(currentPosition) && a.second->isActive)
		{
			if (a.second->mCurrentState == GUIState::Hover)
			{
				a.second->onLeave();
			}
		}

	}
}

GUIInterface* GUIManager::addInterfaceFromFile(State state, std::string file)
{
	std::ifstream fileStream(file);
	std::string name;
	fileStream >> name;
	GUIInterface* interface = new GUIInterface(this, name);
	while (std::getline(fileStream, name))
	{
		std::stringstream ss(name);
		interface->addElement(ss);
	}
}

void GUIManager::handleClick(EventInfo& info)
{
	for (auto& a : mGuiMappings[mCurrentState])
	{
		if (a.second->isWithin(info.mousePosition) && a.second->isActive)
		{
			a.second->onClick(info.mousePosition);
		}
	}
}

void GUIManager::handleRelease(EventInfo& info)
{
	for (auto& a : mGuiMappings[mCurrentState])
	{
		if (a.second->isWithin(info.mousePosition) && a.second->isActive && a.second->mCurrentState == GUIState::Click)
		{
			a.second->onRelease();
		}
	}
}

GUIManager::GUIManager(Context Cntext, EventHandler* handler)
	:sharedContext(Cntext)
{
	registerElement<GUILabel>(GUIElementType::Label);
	handler->registerCallback(State::GameState, "OnClick", &GUIManager::handleClick, this);
	handler->registerCallback(State::GameState, "OnRelease", &GUIManager::handleRelease, this);
}