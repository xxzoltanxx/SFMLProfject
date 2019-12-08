#pragma once
#include <queue>
#include <unordered_map>
#include "GUIElement.h"
#include "State.h"
#include "StateManager.h"

using GUIInterfaces = std::unordered_map<std::string, GUIInterface*>;
using GUIStateMappings = std::unordered_map<State, GUIInterfaces>;
struct EventInfo;
class GUIManager
{
public:
	GUIManager(Context sharedContext, EventHandler* handler);
	void setCurrentState(State state);
	void addInterface(State state, GUIInterface* interface, std::string name);

	void update(float dt);
	void handleClick(EventInfo& info);
	void handleRelease(EventInfo& info);
	void draw(sf::RenderTarget& target);

	void addEvent(const GUIEvent& event);
	bool pollEvent(GUIEvent& event);

	Context& getContext() { return sharedContext; }

	GUIElement* constructElement(GUIElementType type, std::string elementName, GUIInterface* interface)
	{
		return mElementFactory[type](elementName, interface);
	}

	

private:
	template <typename T> void registerElement(GUIElementType type)
	{
		mElementFactory[type] = [](std::string elementName, GUIInterface* inter) -> GUIElement * {return new T(elementName, inter); };
	}

	std::queue<GUIEvent> mGuiEvents;
	GUIStateMappings mGuiMappings;
	State mCurrentState;
	Context sharedContext;

	std::unordered_map < GUIElementType, std::function < GUIElement* (std::string, GUIInterface*) >> mElementFactory;
};