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

	Controllers::CanvasControllers m_Controllers;
	CanvasElements m_Elements;

	EventQueue& m_EventQueue;
	CanvasCamera m_Camera;
};