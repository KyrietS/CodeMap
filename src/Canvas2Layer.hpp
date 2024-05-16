#pragma once
#include "Layer.hpp"
#include "Events/MouseEvents.hpp"

class Canvas2;

class Canvas2Layer : public Layer
{
public:
	Canvas2Layer(Canvas2&);

	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void OnMouseMoved(const Events::Input::MouseMoved&);

	Canvas2& m_Canvas;
};

