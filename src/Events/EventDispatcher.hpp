#pragma once
#include "Event.hpp"

class EventDispatcher
{
public:
	explicit EventDispatcher(Event& event) : m_Event(event) {}

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
private:
	Event& m_Event;
};
