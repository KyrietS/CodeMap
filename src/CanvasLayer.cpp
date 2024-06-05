#include "pch.hpp"
#include "CanvasLayer.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Events/EventQueue.hpp"
#include "Input.hpp"

CanvasLayer::CanvasLayer(CanvasElements& elements, EventQueue& eventQueue)
	: m_Canvas(elements, eventQueue)
{
}

void CanvasLayer::OnUpdate()
{
	m_Canvas.Draw();
}

void CanvasLayer::OnEvent(Event& event)
{
	Input::OnEvent(event); // Set global input state so scripts can use it
	m_Canvas.OnEvent(event);
}

