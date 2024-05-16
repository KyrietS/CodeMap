#include "pch.hpp"
#include "Canvas2Layer.hpp"
#include "Canvas2/Canvas2.hpp"
#include "Input.hpp"

Canvas2Layer::Canvas2Layer(Canvas2& canvas)
	: m_Canvas(canvas)
{
}

void Canvas2Layer::OnUpdate()
{
	m_Canvas.Draw();
}

void Canvas2Layer::OnEvent(Event& event)
{
	Input::OnEvent(event); // Set global input state so scripts can use it
	m_Canvas.OnEvent(event);
}

