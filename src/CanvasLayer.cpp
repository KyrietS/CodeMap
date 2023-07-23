#include "pch.hpp"
#include "CanvasLayer.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"

CanvasLayer::CanvasLayer(Canvas& canvas)
	: m_Canvas(canvas)
{
}

void CanvasLayer::OnUpdate()
{
	m_Canvas.OnUpdate();
	m_Canvas.Draw();
}

void CanvasLayer::OnEvent(Event& event)
{
	Input::OnEvent(event); // Set global input state so scripts can use it
	m_Canvas.OnEvent(event);
}