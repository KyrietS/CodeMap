#include "pch.hpp"
#include "SelectionController.hpp"
#include "Render/VColor.hpp"
#include "Render/Renderer.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/GuiEvents.hpp"
#include "Events/EventDispatcher.hpp"
#include "Canvas/Elements/TextElement.hpp"
#include <clip.h>

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
		dispatcher.Handle<Events::Canvas::Copy>(BIND_EVENT(OnCopy));
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

	bool SelectionController::OnCopy(const Events::Canvas::Copy& event)
	{
		auto selectedElements = GetSelectedElements();
		if (selectedElements.size() == 1)
		{
			if (auto* textElement = m_Elements.TryGet<Elements::TextElement>(selectedElements[ 0 ]))
			{
				bool success = clip::set_text(textElement->GetData().GetTextInUtf8());
				LOG_DEBUG("Copied text to clipboard: {}, success: {}", textElement->GetData().GetTextInUtf8(), success);
				return true;
			}
		}
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