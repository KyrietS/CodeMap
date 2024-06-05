#include "pch.hpp"
#include "ControlPoint.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Render/Renderer.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "Input.hpp"

namespace Elements
{
	void ControlPoint::Draw(const CanvasCamera& camera) const
	{
		float radius = Radius / camera.GetZoom();

		glm::vec4 backgroundColor = Hovering ? VColor::Blue : BackgroundColor;
		backgroundColor = Dragging ? VColor::Red : backgroundColor;

		Renderer::DrawCircle(Position, radius, backgroundColor);
		Renderer::DrawCircleOutline(Position, radius, VColor::Blue);
	}

	void ControlPoint::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(OnMousePressed));
		dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(OnMouseReleased));
		OnUpdate(event);
	}
	
	bool ControlPoint::OnMousePressed(const Events::Input::MousePressed& event)
	{
		const auto mousePos = Input::GetWorldMousePosition(m_Camera);
		if (event.GetButton() == Mouse::ButtonLeft)
		{
			if (Contains(mousePos))
			{
				Dragging = true;
				return true;
			}
		}

		return false;
	}
	
	bool ControlPoint::OnMouseReleased(const Events::Input::MouseReleased& event)
	{
		if (event.GetButton() == Mouse::ButtonLeft)
		{
			if (Dragging and not Input::IsMouseButtonClicked(Mouse::ButtonLeft))
			{
				 m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
			}
			Dragging = false;
		}

		return false;
	}

	void ControlPoint::OnUpdate(Event&)
	{
		const auto mousePos = Input::GetWorldMousePosition(m_Camera);
		Hovering = Contains(mousePos);
		if (Dragging)
		{
			Position = mousePos;
		}
	}
}