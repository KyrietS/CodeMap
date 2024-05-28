#pragma once
#include "Events/EventQueue.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"

class Canvas2
{
public:
	Canvas2(EventQueue&);

	void Draw();
	void OnEvent(Event&);

private:
	void DrawGrid();

	CanvasElements m_Elements;
	Controllers::CanvasControllers m_Controllers;

	EventQueue& m_EventQueue;
	CanvasCamera m_Camera;
};