#pragma once

#include "Events/Event.hpp"
#include "MouseCodes.hpp"


class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float mouseX, float mouseY)
		: m_MouseX(mouseX), m_MouseY(mouseY) {}

	float GetX() { return m_MouseX; }
	float GetY() { return m_MouseY; }

	EventType GetEventType() override {	return GetStaticEventType(); }
	static EventType GetStaticEventType() {	return EventType::MouseMoved; }

private:
	float m_MouseX;
	float m_MouseY;
};

class MousePressedEvent : public Event
{
public:
	MousePressedEvent(MouseCode code) : m_MouseCode(code) {}

	MouseCode GetButton() { return m_MouseCode; }

	EventType GetEventType() override { return GetStaticEventType(); }
	static EventType GetStaticEventType() { return EventType::MouseButtonPressed; }

private:
	MouseCode m_MouseCode;
};

class MouseReleasedEvent : public Event
{
public:
	MouseReleasedEvent(MouseCode code) : m_MouseCode(code) {}

	MouseCode GetButton() { return m_MouseCode; }

	EventType GetEventType() override { return GetStaticEventType(); }
	static EventType GetStaticEventType() { return EventType::MouseButtonReleased; }

private:
	MouseCode m_MouseCode;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

	float GetXOffset() { return m_XOffset; }
	float GetYOffset() { return m_YOffset; }

	EventType GetEventType() override { return GetStaticEventType(); }
	static EventType GetStaticEventType() { return EventType::MouseScrolled; }

private:
	float m_XOffset;
	float m_YOffset;
};