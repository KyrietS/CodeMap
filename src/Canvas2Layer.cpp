#include "pch.hpp"
#include "Canvas2Layer.hpp"
#include "Canvas2/Canvas2.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Events/EventQueue.hpp"
#include "Input.hpp"

Canvas2Layer::Canvas2Layer(CanvasElements& elements, EventQueue& eventQueue)
	: m_Canvas(elements, eventQueue)
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

