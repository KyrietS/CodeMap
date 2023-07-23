#pragma once

#include "Events/Event.hpp"
#include "MouseCodes.hpp"

namespace Events::Input
{

	class MouseMoved {
	public:
		MouseMoved(float mouseX, float mouseY)
				: m_MouseX(mouseX), m_MouseY(mouseY) {}

		float GetX() { return m_MouseX; }

		float GetY() { return m_MouseY; }

	private:
		float m_MouseX;
		float m_MouseY;
	};

	class MousePressed {
	public:
		MousePressed(MouseCode code) : m_MouseCode(code) {}

		MouseCode GetButton() { return m_MouseCode; }

	private:
		MouseCode m_MouseCode;
	};

	class MouseReleasedEvent {
	public:
		MouseReleasedEvent(MouseCode code) : m_MouseCode(code) {}

		MouseCode GetButton() { return m_MouseCode; }

	private:
		MouseCode m_MouseCode;
	};

	class MouseScrolled {
	public:
		MouseScrolled(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() { return m_XOffset; }

		float GetYOffset() { return m_YOffset; }

	private:
		float m_XOffset;
		float m_YOffset;
	};
}