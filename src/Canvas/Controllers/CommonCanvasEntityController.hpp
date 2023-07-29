#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Canvas/Entity.hpp"

class CommonCanvasEntityController : public IController
{
public:
	CommonCanvasEntityController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;

private:
	void OnUpdate(Entity);

	EventQueue& m_EventQueue;
};
