#pragma once
#include <functional>
#include <any>

#define BIND_EVENT(callback) [this](auto& event) { return this->callback(event); }

struct EmptyEvent {};

class Event
{
public:
	template<typename T>
	Event(T event) : m_Event(event) {} // NOLINT(*-explicit-constructor)

	template<typename T>
	bool IsType() const { return m_Event.type() == typeid(T); }

	template<typename T>
	T& GetEvent() { return std::any_cast<T&>(m_Event); }
	template<typename T>
	const T& GetEvent() const { return std::any_cast<const T&>(m_Event); }

	const std::type_info& GetEventType() const { return m_Event.type(); }

	bool Handled = false;
private:
	std::any m_Event;
};
