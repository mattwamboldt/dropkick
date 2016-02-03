#include "eventmanager.h"
#include <SDL\SDL.h>

EventManager* gEventManager = NULL;

EventManager::EventManager(const char* name, bool setAsGlobal)
{
	mActiveQueue = 0;
	if (setAsGlobal)
	{
		if (gEventManager)
		{
			delete gEventManager;
		}

		gEventManager = this;
	}
}

EventManager::~EventManager()
{

}

bool EventManager::AddListener(const EventListener& listener, const EventType& type)
{
	ListenerList& listeners = mListeners[type];
	for (auto iter = listeners.begin(); iter != listeners.end(); ++iter)
	{
		if (listener == (*iter))
		{
			//TODO: Add warning/logging system
			//WARNING: "Double registering a delegate."
			return false;
		}
	}

	listeners.push_back(listener);
	return true;
}

bool EventManager::RemoveListener(const EventListener& listener, const EventType& type)
{
	ListenerMap::iterator mapIter = mListeners.find(type);
	if (mapIter != mListeners.end())
	{
		ListenerList& listeners = mapIter->second;
		for (auto iter = listeners.begin(); iter != listeners.end(); ++iter)
		{
			if (listener == (*iter))
			{
				// We shouldn't be able to register multiple times, so we jump out here
				listeners.erase(iter);
				return true;
			}
		}
	}

	return false;
}

bool EventManager::Trigger(const IEventPtr& eventData) const
{
	bool handled = false;
	auto mapIter = mListeners.find(eventData->GetType());
	if (mapIter != mListeners.end())
	{
		const ListenerList& listeners = mapIter->second;
		for (auto iter = listeners.begin(); iter != listeners.end(); ++iter)
		{
			EventListener listener = (*iter);
			listener(eventData);
			handled = true;
		}
	}

	return handled;
}

bool EventManager::Queue(const IEventPtr& eventData)
{
	// TODO: Throw some asserts about the active queue here

	// If a tree falls in the forrest and no ones around, it doesn't make a sound
	auto mapIter = mListeners.find(eventData->GetType());
	if (mapIter != mListeners.end())
	{
		mQueues[mActiveQueue].push_back(eventData);
		return true;
	}

	return false;
}

bool EventManager::Abort(const EventType& type, bool all)
{
	bool removed = false;
	// Make sure we have listeners before we abort
	if (mListeners.find(type) != mListeners.end())
	{
		// TODO: throw in some asserts about the active queue
		EventQueue& queue = mQueues[mActiveQueue];
		auto iter = queue.begin();

		while (iter != queue.end())
		{
			// Use a surrogate iterator for removal so we don't invalidate the actual iterator
			auto thisIter = iter;
			++iter;

			if ((*thisIter)->GetType() == type)
			{
				queue.erase(thisIter);
				removed = true;
				if (!all) break;
			}
		}
	}

	return removed;
}

bool EventManager::Update(unsigned long maxTime)
{
	unsigned long currentMS = SDL_GetTicks();
	unsigned long maxMS = ((maxTime == eINFINITE) ? (eINFINITE) : (currentMS + maxTime));

	// Move to the next queue
	int currentQueue = mActiveQueue;
	mActiveQueue = (mActiveQueue + 1) % NUM_EVENT_QUEUES;
	mQueues[mActiveQueue].clear();

	// Process the current queue as much as possible
	while (!mQueues[currentQueue].empty())
	{
		IEventPtr eventData = mQueues[currentQueue].front();
		mQueues[currentQueue].pop_front();

		Trigger(eventData);

		// Jump out early if we're out of time
		currentMS = SDL_GetTicks();
		if (maxMS != eINFINITE && currentMS > maxMS)
		{
			// TODO: Log that we are aborting processing due to time
			break;
		}
	}

	// Move unprocessed events to the active queue if the current queue wasn't fully processed
	// Note: to preserve order we go back to front inserting at the head of the queue
	bool flushed = mQueues[currentQueue].empty();
	if (!flushed)
	{
		while (!mQueues[currentQueue].empty())
		{
			IEventPtr eventData = mQueues[currentQueue].back();
			mQueues[currentQueue].pop_back();
			mQueues[mActiveQueue].push_front(eventData);
		}
	}

	return flushed;
}

EventManager* EventManager::Get()
{
	return gEventManager;
}
