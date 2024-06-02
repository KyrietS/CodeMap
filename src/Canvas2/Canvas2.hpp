#pragma once
#include "Events/EventQueue.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Events/CanvasEvents.hpp"

class Canvas2
{
public:
	Canvas2(CanvasElements&, EventQueue&);

	void Draw();
	void OnEvent(Event&);

private:
	bool OnCanvasSaveToFile(const Events::Canvas::SaveToFile&);
	bool OnCanvasLoadFromFile(const Events::Canvas::LoadFromFile&);
	void DrawGrid();

	Controllers::CanvasControllers m_Controllers;

	CanvasElements& m_Elements;
	EventQueue& m_EventQueue;
	CanvasCamera m_Camera;
};