#ifndef IEVENT_H
#define IEVENT_H

/*
This event system is based on the one in Game Coding Complete and is at the
heart of that engine. An event system allows you to treat sub-systems as being
much more independant. They can act like separate processes that use the event system
as a form of ultra fast ipc. Which in turn means you can separate your game logic from the rest of
the game and do online and journaling functionality way easier.

Why is this class based and not a big enum?
- It allows extra data to be attached to an event, otherwise it's just a software interupt type thing
- It allows events to be serialized for file output, transmission accross the wire, etc.
- Avoids the problem of maintaining a giant enum and the huge recompiles that brings

This system uses guids as the event type, which means we can have multiple event definition files
for different systems, without fear of overlapping ids. Systems only need to include the files for
the events they are interested in.
*/

#include <sstream>

typedef unsigned long EventType;

class IEvent
{
public:
	virtual const EventType& GetType() = 0;
	virtual float GetTimeStamp() = 0;
	virtual void Serialize(std::ostringstream& out) = 0;
	virtual const char* GetName() const = 0;
};

class BaseEvent : public IEvent
{
public:
	BaseEvent(const float timeStamp = 0.0f) :
		mTimeStamp(timeStamp)
	{}

	virtual ~BaseEvent() {}

	float GetTimeStamp() { return mTimeStamp; }
	virtual void Serialize(std::ostringstream& out) {};
private:
	float mTimeStamp;
};

#endif