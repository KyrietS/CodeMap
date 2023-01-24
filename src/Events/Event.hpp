#pragma once
#include <functional>


#define BIND_EVENT(callback) [this](auto& event) { this->callback(event); }


enum class EventType
{
	None = 0,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	KeyPressed, KeyReleased, KeyTyped
};

class Event
{
public:
	virtual ~Event() = default;

	virtual EventType GetEventType() = 0;

	bool Handled = false;
};

template <typename T>
concept EventWithStaticType =
	requires(T) {
		{ T::GetStaticEventType() } -> std::same_as<EventType>;
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event) : m_Event(event) {}

	template<EventWithStaticType T>
	bool Dispatch(const std::function<void(T&)>& onEventCallback)
	{
		if (m_Event.GetEventType() == T::GetStaticEventType())
		{
			onEventCallback(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}
private:
	Event& m_Event;
};