#pragma once

#include "Canvas/Components.hpp"
#include "IController.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventQueue.hpp"
#include "Canvas/Entity.hpp"

class SelectionController : public IController
{
public:
	SelectionController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;

private:
	EventQueue& m_EventQueue;
};