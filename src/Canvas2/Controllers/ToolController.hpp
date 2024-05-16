#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/CanvasEvents.hpp"

namespace Controllers
{
class ToolController : public IController
{
public:
	ToolController(CanvasCamera& camera, EventQueue& eventQueue)
		: m_Camera(camera), m_EventQueue(eventQueue) {}
	void Draw() override;
	void OnEvent(Event& event) override;

private:
	void OnToolSelectedEvent(const Events::Canvas::SelectTool& event);

	CanvasCamera& m_Camera;
	EventQueue& m_EventQueue;
	std::unique_ptr<IController> m_ActiveTool;
};

}
