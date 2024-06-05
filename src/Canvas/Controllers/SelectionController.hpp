#pragma once
#include "IController.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"

namespace Controllers
{
class SelectionController : public IController
{
public:
	SelectionController(CanvasCamera& camera, EventQueue& eventQueue, CanvasElements& elements)
		: m_Camera(camera), m_EventQueue(eventQueue), m_Elements(elements) {}
	void Draw() override;
	void OnEvent(Event&) override;

private:
	bool OnSelectElement(const Events::Canvas::SelectElement&);
	std::vector<ElementId> GetSelectedElements();
	void UnselectAllElements();

	CanvasCamera& m_Camera;
	EventQueue& m_EventQueue;
	CanvasElements& m_Elements;
};

}
