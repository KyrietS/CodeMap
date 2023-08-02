#pragma once

#include "Canvas/Components.hpp"
#include "IController.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventQueue.hpp"

class SelectionController : public IController
{
public:
	SelectionController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;
	void OnEvent(Event& event) override;

private:
	void OnSetFocus(const Events::Canvas::SetFocus&);

	EventQueue& m_EventQueue;
};