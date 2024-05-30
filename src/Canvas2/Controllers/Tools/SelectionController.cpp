#include "pch.hpp"
#include "SelectionController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "Canvas2/Elements/TextElement.hpp"

namespace Controllers
{
	SelectionController::~SelectionController()
	{
		UnselectAllElements();
	}

	void SelectionController::Draw()
	{
		if (m_HoveredElement.has_value())
		{
			if (auto* element = m_Elements.TryGet(*m_HoveredElement))
			{
				const auto box = element->GetBoundingBox();
				const float thickness = 1.0f / m_Camera.GetZoom();
				Renderer::DrawRectangleLines({ box.x, box.y }, box.width, box.height, thickness, VColor::Red);
			}
		}

		for (const auto id : m_SelectedElements)
		{
			if (auto* element = m_Elements.TryGet(id))
			{
				const auto box = element->GetBoundingBox();
				const float thickness = 1.0f / m_Camera.GetZoom();
				Renderer::DrawRectangleLines({ box.x, box.y }, box.width, box.height, thickness, VColor::Blue);
			}
		}

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
		// We give the selected elements a chance to handle the event first
		PassEventToSelectedElements(event);
		if (event.Handled)
			return;

		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(SelectionController::OnMousePressed));
		dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(SelectionController::OnMouseReleased));
		dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(SelectionController::OnKeyPressed));

		OnUpdate();
	}

	void SelectionController::OnUpdate()
	{
		if (m_SelectionStart.has_value())
		{
			m_SelectionEnd = Input::GetWorldMousePosition(m_Camera);
		}

		if (m_MoveByDrag)
		{
			glm::vec2 delta = Input::GetWorldMousePosition(m_Camera) - m_LastMouseWorldPosition;
			MoveSelectedElementsBy(delta);
		}
		else
		{
			HandleMouseHoveredOverElement();
		}

		m_LastMouseWorldPosition = Input::GetWorldMousePosition(m_Camera);
	}

	bool SelectionController::OnMousePressed(const Events::Input::MousePressed& event)
	{
		if (event.GetButton() != Mouse::ButtonLeft)
			return false;

		if (SelectHoveredElement())
		{
			m_MoveByDrag = true;
			m_LastMouseWorldPosition = Input::GetWorldMousePosition(m_Camera);
			return true;
		}

		m_SelectionStart = Input::GetWorldMousePosition(m_Camera);
		return true;
	}

	bool SelectionController::OnMouseReleased(const Events::Input::MouseReleased& event)
	{
		if (event.GetButton() == Mouse::ButtonLeft)
		{
			m_MoveByDrag = false;
			HideSelectionRectangle();
			return true;
		}
		return false;
	}

	bool SelectionController::OnKeyPressed(const Events::Input::KeyPressed& event)
	{
		if (event.GetKey() == Key::Delete)
		{
			RemoveSelectedElements();
			return true;
		}
		return false;
	}

	void SelectionController::PassEventToSelectedElements(Event& event)
	{
		for (auto id : m_SelectedElements)
		{
			if (event.Handled)
				return;

			if (auto* element = m_Elements.TryGet(id))
			{
				element->OnEvent(event);
			}
		}
	}

	static void DisableEditModeWhenThereAreMultipleTextElements(CanvasElements& elements, const std::set<ElementId>& selectedElements)
	{
		if (selectedElements.size() > 1)
		{
			for (auto id : selectedElements)
			{
				if (auto* textElement = elements.TryGet<Elements::TextElement>(id))
				{
					textElement->InEditMode = false;
				}
			}
		}
	}

	bool SelectionController::SelectHoveredElement()
	{
		auto mousePos = Input::GetWorldMousePosition(m_Camera);
		for (const auto& [id, element] : m_Elements)
		{
			if (element->Contains(mousePos))
			{
				m_SelectedElements.insert(id);
				element->InEditMode = true;
				DisableEditModeWhenThereAreMultipleTextElements(m_Elements, m_SelectedElements);
				return true;
			}
		}

		// Clicked on empty space
		UnselectAllElements();
		return false;
	}

	void SelectionController::HandleMouseHoveredOverElement()
	{
		m_HoveredElement.reset();

		glm::vec2 mousePosition = Input::GetWorldMousePosition(m_Camera);
		for (const auto& [id, element] : m_Elements)
		{
			if (element->Contains(mousePosition))
			{
				m_HoveredElement = id;
				return;
			}
		}
	}

	void SelectionController::MoveSelectedElementsBy(glm::vec2 delta)
	{
		for (auto id : m_SelectedElements)
		{
			if (auto* element = m_Elements.TryGet(id))
			{
				element->MoveBy(delta.x, delta.y);
			}
		}
	}

	void SelectionController::UnselectAllElements()
	{
		for (auto id : m_SelectedElements)
		{
			if (auto* element = m_Elements.TryGet(id))
			{
				element->InEditMode = false;
			}
		}
		m_SelectedElements.clear();
	}

	void SelectionController::RemoveSelectedElements()
	{
		for (auto id : m_SelectedElements)
		{
			m_Elements.Remove(id);
		}
		m_SelectedElements.clear();
	}

	void SelectionController::HideSelectionRectangle()
	{
		m_SelectionStart.reset();
		m_SelectionEnd.reset();
	}

}
