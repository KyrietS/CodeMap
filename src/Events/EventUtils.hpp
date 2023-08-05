#pragma once

class Event;

namespace Events
{
	bool IsMouseEvent(const Event& event);
	bool IsKeyEvent(const Event& event);
}