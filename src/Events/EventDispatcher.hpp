#pragma once
#include "Event.hpp"

class EventDispatcher
{
public:
	explicit EventDispatcher(Event& event) : m_Event(event) {}

	// Dispatches the event to the callback if the event is of type T
	template<typename T>
	bool Dispatch(const std::function<void(T&)>& onEventCallback)
	{
		if (m_Event.IsType<T>())
		{
			onEventCallback(m_Event.GetEvent<T>());
			return true;
		}
		return false;
	}

	// Dispatches the event to the callback and marks in as handled
	// if the callback returns true
	template<typename T>
	bool Handle(const std::function<bool(T&)>& onEventCallback)
	{
		if (m_Event.IsType<T>())
		{
			m_Event.Handled |= onEventCallback(m_Event.GetEvent<T>());
			return true;
		}
		return false;
	}
private:
	Event& m_Event;
};
