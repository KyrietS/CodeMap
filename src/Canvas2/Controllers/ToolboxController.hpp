#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/KeyEvents.hpp"
#include "Canvas2/CanvasElements.hpp"

namespace Controllers
{
class ToolboxController : public IController
{
public:
	ToolboxController(CanvasCamera& camera, EventQueue& eventQueue, CanvasElements& elements)
		: m_Camera(camera), m_EventQueue(eventQueue), m_Elements(elements) {}
	void Draw() override;
	void OnEvent(Event& event) override;

private:
	void OnToolSelectedEvent(const Events::Canvas::SelectTool& event);
	bool OnKeyPressed(const Events::Input::KeyPressed& event);

	CanvasCamera& m_Camera;
	EventQueue& m_EventQueue;
	CanvasElements& m_Elements;
	std::unique_ptr<IController> m_ActiveTool;
};

}
