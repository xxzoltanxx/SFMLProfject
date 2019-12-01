#pragma once
#include <vector>
#include <unordered_map>
#include "Observer.h"

class Subscriptions
{
public:
	void addSubscriber(Observer* obs);
	void removeSubscriber(Observer* obs);
	void notifyObservers(const ObserverPayload& payload);
private:
	std::vector<Observer*> observers;
};

class MessageHandler
{
public:
	void dispatchMessage(const ObserverPayload& payload);
	void addSubscriber(ObserverMessage msg, Observer* obs);
	void removeSubscriber(ObserverMessage msg, Observer* obs);
private:
	std::unordered_map<ObserverMessage, Subscriptions> mMessageMapping;
};