#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Canvas/Entity.hpp"
#include "Events/CanvasEvents.hpp"

class CommonCanvasEntityController : public IController
{
public:
	CommonCanvasEntityController(EventQueue& eventQueue)
		: m_EventQueue(eventQueue) {}
	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void OnUpdate(Entity);
	void OnSetFocus(const Events::Canvas::SetFocus&);
	void OnClearFocus(const Events::Canvas::ClearFocus&);
	void FocusOn(Entity);
	void UnfocusAllEntities();

	EventQueue& m_EventQueue;
};
