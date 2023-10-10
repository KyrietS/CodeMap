#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"

class HighlightController : public IController
{
public:
	HighlightController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;

private:
	EventQueue& m_EventQueue;
};

