#pragma once

class Event;

class IController
{
public:
	virtual ~IController() = default;

	virtual void OnUpdate() {};
	virtual void OnEvent(Event&) {};
};