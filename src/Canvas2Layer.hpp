#pragma once
#include "Layer.hpp"
#include "Events/MouseEvents.hpp"
#include "Canvas2/Canvas2.hpp"

class EventQueue;
class CanvasElements;

class Canvas2Layer : public Layer
{
public:
	Canvas2Layer(CanvasElements&, EventQueue&);

	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void OnMouseMoved(const Events::Input::MouseMoved&);

	Canvas2 m_Canvas;
};

