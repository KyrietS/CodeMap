#pragma once

class Event;

class GuiElement
{
public:
	GuiElement() = default;
	virtual ~GuiElement() = default;
	virtual void OnUpdate() = 0;
	virtual void OnEvent(Event&) {};
};