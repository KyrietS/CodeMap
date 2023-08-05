#include "pch.hpp"
#include "Events/EventUtils.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"

namespace Events
{
	bool IsMouseEvent(const Event& event)
	{
		return event.IsType<Events::Input::MousePressed>() ||
			event.IsType<Events::Input::MouseReleased>() ||
			event.IsType<Events::Input::MouseMoved>() ||
			event.IsType<Events::Input::MouseScrolled>();
	}

	bool IsKeyEvent(const Event& event)
	{
		return event.IsType<Events::Input::KeyPressed>() ||
			event.IsType<Events::Input::KeyReleased>() ||
			event.IsType<Events::Input::KeyTyped>();
	}
}