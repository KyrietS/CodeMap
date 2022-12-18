#pragma once

#include "Events/Event.hpp"
#include "KeyCodes.hpp"

class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(KeyCode key, bool repeated = false) : m_KeyCode(key), m_Repeated(repeated) {}

	KeyCode GetKey() { return m_KeyCode; }
	bool IsRepeated() { return m_Repeated; }

	EventType GetEventType() override { return GetStaticEventType(); }
	static EventType GetStaticEventType() { return EventType::KeyPressed; }

private:
	KeyCode m_KeyCode;
	bool m_Repeated;
};

class KeyReleasedEvent : public Event
{
public:
	KeyReleasedEvent(KeyCode key) : m_KeyCode(key) {}

	KeyCode GetKey() { return m_KeyCode; }

	EventType GetEventType() override { return GetStaticEventType(); }
	static EventType GetStaticEventType() { return EventType::KeyReleased; }

private:
	KeyCode m_KeyCode;
};

class KeyTypedEvent : public Event
{
public:
	KeyTypedEvent(uint32_t codePoint) : m_CodePoint(codePoint) {}

	uint32_t GetCodePoint() { return m_CodePoint; }

	EventType GetEventType() override { return GetStaticEventType(); }
	static EventType GetStaticEventType() { return EventType::KeyTyped; }

private:
	uint32_t m_CodePoint;
};