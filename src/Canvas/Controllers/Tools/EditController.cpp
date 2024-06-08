#include "pch.hpp"
#include "EditController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"
#include "Render/Renderer.hpp"
#include "Render/VColor.hpp"
#include "Canvas/Box.hpp"
#include "Canvas/Elements/TextElement.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas/Elements/ArrowElement.hpp"

namespace Controllers
{
	EditController::~EditController()
	{
		UnselectAllElements();
	}

	void EditController::Draw()
	{
		if (m_HoveredElement.has_value())
		{
			auto* element = m_Elements.TryGet(*m_HoveredElement);
			if (element and not element->InEditMode)
			{
				const auto box = element->GetBoundingBox();
				const float thickness = 1.0f / m_Camera.GetZoom();
				Renderer::DrawRectangleLines({ box.x, box.y }, box.width, box.height, thickness, VColor::Red);
			}
		}

		if (m_SelectionStart.has_value() && m_SelectionEnd.has_value())
		{
			DrawSelection(m_SelectionStart.value(), m_SelectionEnd.value());
		}

		for (auto& [id, element] : m_Elements)
		{
			if (element->InEditMode)
			{
				if (auto* arrow = element->As<Elements::ArrowElement>())
				{
					for (const auto& point : arrow->GetData().Points)
					{
						point.Draw(m_Camera);
						// TODO: Implement for other elements
						//DrawControlPoint(point, m_Camera);
					}
				}
			}
		}
	}

	void EditController::DrawSelection(const glm::vec2& begin, const glm::vec2& end)
	{
		Box selection = Box::Create({ begin, end });
		float thickness = 1.0f / m_Camera.GetZoom();
		glm::vec4 color = VColor::Blue;

		Renderer::DrawRectangleLines({selection.x, selection.y}, selection.width, selection.height, thickness, color);
	}

	void EditController::OnEvent(Event& event)
	{
		// We give the selected elements a chance to handle the event first
		PassEventToSelectedElements(event);
		if (event.Handled)
			return;

		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(EditController::OnMousePressed));
		dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(EditController::OnMouseReleased));
		dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(EditController::OnKeyPressed));

		OnUpdate();
	}

	void EditController::OnUpdate()
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

	bool EditController::OnMousePressed(const Events::Input::MousePressed& event)
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

	bool EditController::OnMouseReleased(const Events::Input::MouseReleased& event)
	{
		if (event.GetButton() == Mouse::ButtonLeft)
		{
			if (m_MoveByDrag and not Input::IsMouseButtonClicked(Mouse::ButtonLeft))
			{
				m_EventQueue.Push(Events::Canvas::MakeSnapshot {});
			}

			m_MoveByDrag = false;
			SelectElementsInsideSelectionRectangle();
			HideSelectionRectangle();
			return true;
		}
		return false;
	}

	bool EditController::OnKeyPressed(const Events::Input::KeyPressed& event)
	{
		if (event.GetKey() == Key::Delete)
		{
			RemoveSelectedElements();
			return true;
		}
		return false;
	}

	void EditController::PassEventToSelectedElements(Event& event)
	{
		for (auto& [id, element] : m_Elements)
		{
			if (event.Handled)
				return;

			if (element->InEditMode)
			{
				element->OnEvent(event);
			}
		}
	}

	bool EditController::SelectHoveredElement()
	{
		auto mousePos = Input::GetWorldMousePosition(m_Camera);
		for (const auto& [id, element] : m_Elements)
		{
			if (element->Contains(mousePos))
			{
				HandleMousePressedOnElement(id, *element);
				return true;
			}
		}

		// Clicked on empty space
		UnselectAllElements();
		return false;
	}

	void EditController::HandleMousePressedOnElement(ElementId elementId, const Elements::IElement& element)
	{
		if (not element.InEditMode)
		{
			const bool multiSelect = Input::IsKeyDown(Key::LeftControl);
			m_EventQueue.Push(Events::Canvas::SelectElement { elementId, multiSelect });
		}
	}

	void EditController::HandleMouseHoveredOverElement()
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

	void EditController::MoveSelectedElementsBy(glm::vec2 delta)
	{
		for (auto& [id, element] : m_Elements)
		{
			if (element->InEditMode)
			{
				element->MoveBy(delta.x, delta.y);
			}
		}
	}

	void EditController::UnselectAllElements()
	{
		m_EventQueue.Push(Events::Canvas::SelectElement { 0 });
	}

	void EditController::RemoveSelectedElements()
	{
		m_Elements.RemoveIf([](const Elements::IElement& element) { return element.InEditMode; });
	}

	void EditController::SelectElementsInsideSelectionRectangle()
	{
		if (not m_SelectionStart.has_value() or not m_SelectionEnd.has_value())
			return;

		Box selection = Box::Create({ m_SelectionStart.value(), m_SelectionEnd.value() });
		for (auto& [id, element] : m_Elements)
		{
			if (selection.Contains(element->GetBoundingBox()))
			{
				m_EventQueue.Push(Events::Canvas::SelectElement { .ElementId = id, .MultiSelect = true });
			}
		}
	}

	void EditController::HideSelectionRectangle()
	{
		m_SelectionStart.reset();
		m_SelectionEnd.reset();
	}
}
