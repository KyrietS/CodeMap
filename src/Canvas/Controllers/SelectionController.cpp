#include "pch.hpp"
#include "SelectionController.hpp"
#include "Render/VColor.hpp"
#include "Render/Renderer.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/GuiEvents.hpp"
#include "Events/EventDispatcher.hpp"

namespace Controllers
{
	void SelectionController::Draw()
	{
		for (auto& [id, element] : m_Elements)
		{
			if (element->InEditMode)
			{
				const auto box = element->GetBoundingBox();
				const float thickness = 1.0f / m_Camera.GetZoom();
				Renderer::DrawRectangleLines({ box.x, box.y }, box.width, box.height, thickness, VColor::Blue);
			}
		}
	}

	void SelectionController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Canvas::SelectElement>(BIND_EVENT(OnSelectElement));
	}

	bool SelectionController::OnSelectElement(const Events::Canvas::SelectElement& event)
	{
		if (not event.MultiSelect)
		{
			UnselectAllElements();
		}

		if (auto* element = m_Elements.TryGet(event.ElementId))
		{
			element->InEditMode = true;
		}

		m_EventQueue.Push(Events::Gui::ShowProperties { GetSelectedElements() });
		return false;
	}

	std::vector<ElementId> SelectionController::GetSelectedElements()
	{
		std::vector<ElementId> selectedElements;
		for (auto& [id, element] : m_Elements)
		{
			if (element->InEditMode)
			{
				selectedElements.push_back(id);
			}
		}
		return selectedElements;
	}

	void SelectionController::UnselectAllElements()
	{
		for (auto& [id, element] : m_Elements)
		{
			element->InEditMode = false;
		}
	}
}