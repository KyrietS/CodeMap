#include "pch.hpp"
#include "ShapeController.hpp"
#include "Canvas2/Elements/ShapeElement.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"

namespace Controllers
{
	ShapeController::ShapeController(CanvasCamera& camera, CanvasElements& elements)
		: m_Camera(camera), m_Elements(elements)
	{
	}

	void ShapeController::Draw()
	{
		if (m_Shape)
		{
			m_Shape->Draw();
		}
	}

	void ShapeController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(OnMousePressed));
		dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(OnMouseReleased));
	
		OnUpdate();
	}

	void ShapeController::OnUpdate()
	{
		if (m_Shape)
		{
			glm::vec2 mousePos = Input::GetWorldMousePosition(m_Camera);
			const glm::vec2 origin = m_Shape->GetData().Points.at(0).Position;
			glm::vec2& cornerA = m_Shape->GetData().Points.at(1).Position;
			glm::vec2& cornerB = m_Shape->GetData().Points.at(2).Position;
			glm::vec2& cornerC = m_Shape->GetData().Points.at(3).Position;

			cornerA = { mousePos.x, origin.y };
			cornerB = mousePos;
			cornerC = { origin.x, mousePos.y };
		}
	}

	bool ShapeController::OnMousePressed(Events::Input::MousePressed& event)
	{
		if (event.GetButton() != Mouse::ButtonLeft)
			return false;

		if (not m_Shape)
		{
			m_Shape = std::make_unique<Elements::ShapeElement>(m_Camera);

			// rectangle by default for now
			glm::vec2 begin = Input::GetWorldMousePosition(m_Camera);
			m_Shape->AddPoint(begin);
			m_Shape->AddPoint(begin);
			m_Shape->AddPoint(begin);
			m_Shape->AddPoint(begin);
		}

		return true;

	}

	bool ShapeController::OnMouseReleased(Events::Input::MouseReleased& event)
	{
		if (event.GetButton() != Mouse::ButtonLeft)
			return false;

		if (m_Shape and not Input::IsMouseButtonClicked(Mouse::ButtonLeft))
		{
			AddShapeToCanvas();
		}

		return true;
	}

	void ShapeController::AddShapeToCanvas()
	{
		if (m_Shape and m_Shape->GetBoundingBox().width > 5.0f and m_Shape->GetBoundingBox().height > 5.0f)
		{
			LOG_DEBUG("Shape added to canvas");
			m_Elements.Add(std::move(m_Shape));
		}
		m_Shape.reset();
	}

}