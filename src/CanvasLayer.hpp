#pragma once
#include "Layer.hpp"
#include "Events/MouseEvents.hpp"
#include "Canvas/Canvas.hpp"

class EventQueue;
class CanvasElements;

class CanvasLayer : public Layer
{
public:
	CanvasLayer(CanvasElements&, EventQueue&);

	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void OnMouseMoved(const Events::Input::MouseMoved&);

	Canvas m_Canvas;
};

