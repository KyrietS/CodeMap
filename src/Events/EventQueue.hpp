#pragma once
#include <queue>
#include "Event.hpp"

class EventQueue
{
public:
	EventQueue() = default;
	EventQueue(const EventQueue&) = delete;

	template<typename T>
	void Push(const T& event)
	{
		Event& e = m_QueuedEvents.emplace(event);
		LOG_TRACE("[EventQueue] Pushed event of type: {}", e.GetEventType().name());
	}

	Event Pop()
	{
		if (Empty())
			return Event(EmptyEvent{});

		Event event = m_QueuedEvents.front();
		m_QueuedEvents.pop();
		return event;
	}

	bool Empty() const { return m_QueuedEvents.empty(); }

	std::queue<Event> m_QueuedEvents;
};