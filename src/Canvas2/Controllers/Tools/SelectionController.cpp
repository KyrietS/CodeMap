#include "pch.hpp"
#include "SelectionController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"

namespace Controllers
{
	void SelectionController::Draw()
	{
		if (m_SelectionStart.has_value() && m_SelectionEnd.has_value())
		{
			DrawSelection(m_SelectionStart.value(), m_SelectionEnd.value());
		}
	}

	void SelectionController::DrawSelection(const glm::vec2& begin, const glm::vec2& end)
	{
		float minX = std::min(begin.x, end.x);
		float minY = std::min(begin.y, end.y);

		float width = std::abs(begin.x - end.x);
		float height = std::abs(begin.y - end.y);
		float thickness = 1.0f / m_Camera.GetZoom();
		glm::vec4 color = VColor::Blue;

		Renderer::DrawRectangleLines({ minX, minY }, width, height, thickness, color);
	}

	void SelectionController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(SelectionController::OnMousePressed));
		dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(SelectionController::OnMouseReleased));
		dispatcher.Handle<Events::Input::MouseMoved>(BIND_EVENT(SelectionController::OnMouseMoved));
	}

	bool SelectionController::OnMousePressed(const Events::Input::MousePressed& event)
	{
		if (event.GetButton() == Mouse::ButtonLeft)
		{
			m_SelectionStart = Input::GetWorldMousePosition(m_Camera);
			return true;
		}
		return false;
	}

	bool SelectionController::OnMouseReleased(const Events::Input::MouseReleased& event)
	{
		if (event.GetButton() == Mouse::ButtonLeft)
		{
			ClearSelection();
			return true;
		}
		return false;
	}

	bool SelectionController::OnMouseMoved(const Events::Input::MouseMoved& event)
	{
		if (m_SelectionStart.has_value() && Input::IsMouseDragged(Mouse::ButtonLeft))
		{
			m_SelectionEnd = Input::GetWorldMousePosition(m_Camera);
			return true;
		}

		return false;
	}

	void SelectionController::ClearSelection()
	{
		m_SelectionStart.reset();
		m_SelectionEnd.reset();
	}

}
