#include "MessageHandler.h"
void MessageHandler::dispatchMessage(const ObserverPayload& payload)
{
	auto subscription = mMessageMapping.find(ObserverMessage(payload.type));
	if (subscription != mMessageMapping.end())
	{
		subscription->second.notifyObservers(payload);
	}
}

void MessageHandler::addSubscriber(ObserverMessage msg, Observer* obs)
{
	auto subscription = mMessageMapping.find(msg);
	if (subscription == mMessageMapping.end())
	{
		mMessageMapping[msg] = Subscriptions();
	}
	mMessageMapping[msg].addSubscriber(obs);
}

void MessageHandler::removeSubscriber(ObserverMessage msg, Observer* obs)
{
	mMessageMapping[msg].removeSubscriber(obs);
}

void Subscriptions::addSubscriber(Observer* obs)
{
	observers.push_back(obs);
}

void Subscriptions::removeSubscriber(Observer* obs)
{
	auto iter = std::find(observers.begin(), observers.end(), obs);
	observers.erase(iter);
}

void Subscriptions::notifyObservers(const ObserverPayload& payload)
{
	for (auto& a : observers)
	{
		a->notify(payload);
	}
}