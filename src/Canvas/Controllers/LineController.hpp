#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"

class LineController : public IController
{
public:
	LineController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;

private:
	EventQueue& m_EventQueue;
};

