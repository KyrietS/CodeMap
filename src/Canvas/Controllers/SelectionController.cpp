#include "pch.hpp"
#include "SelectionController.hpp"
#include "Render/VColor.hpp"
#include "Render/Renderer.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/GuiEvents.hpp"


namespace Controllers
{
    void SelectionController::Draw()
    {
        std::vector<ElementId> selectedElements;

        for (auto& [id, element] : m_Elements)
        {
            if (element->InEditMode)
            {
                selectedElements.push_back(id);

                const auto box = element->GetBoundingBox();
                const float thickness = 1.0f / m_Camera.GetZoom();
                Renderer::DrawRectangleLines({ box.x, box.y }, box.width, box.height, thickness, VColor::Blue);
            }
        }

        if (selectedElements != m_SelectedElements)
        {
            m_SelectedElements = std::move(selectedElements);
			m_EventQueue.Push(Events::Gui::ShowProperties { m_SelectedElements });
        }
        else if (not m_SelectedElements.empty() and selectedElements.empty())
        {
            m_SelectedElements.clear();
            m_EventQueue.Push(Events::Gui::ShowProperties {});
        }
    }
}