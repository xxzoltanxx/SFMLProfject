#pragma once
enum class ObserverMessage
{
	MOVE
};
struct ObserverPayload
{
	ObserverMessage type;
	int mReceiver;
	union {
		bool mBool;
		float mX, mY;
		int mInt;
	};
};

class Observer
{
public:
	virtual void notify(const ObserverPayload& payload) = 0;
};
