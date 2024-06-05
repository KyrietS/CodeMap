#pragma once
#include "Events/EventQueue.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Controllers/IController.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Events/CanvasEvents.hpp"

class Canvas
{
public:
	Canvas(CanvasElements&, EventQueue&);

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