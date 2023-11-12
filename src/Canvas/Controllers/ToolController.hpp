#pragma once
#include "Canvas/Controllers/IController.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas/CanvasCamera.hpp"

class ToolController : public IController
{
public:
	ToolController(EventQueue& eventQueue, CanvasCamera& canvasCamera) : m_EventQueue(eventQueue), m_Camera(canvasCamera) {}
	void OnUpdate() override;
	void OnEvent(Event& event) override;
private:
	void OnToolSelectedEvent(const Events::Canvas::SelectTool& event);

	std::vector<std::unique_ptr<IController>> m_ActiveTools;
	CanvasCamera& m_Camera;
	EventQueue& m_EventQueue;
};
