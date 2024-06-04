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
        std::set<ElementId> selectedElements;

        for (auto& [id, element] : m_Elements)
        {
            if (element->InEditMode)
            {
                selectedElements.insert(id);

                const auto box = element->GetBoundingBox();
                const float thickness = 1.0f / m_Camera.GetZoom();
                Renderer::DrawRectangleLines({ box.x, box.y }, box.width, box.height, thickness, VColor::Blue);
            }
        }

        if (selectedElements.size() == 1 and *selectedElements.begin() != m_SelectedElement)
        {
            m_SelectedElement = *selectedElements.begin();
			m_EventQueue.Push(Events::Gui::ShowProperties2 { m_SelectedElement });
        }
        else if (m_SelectedElement != 0 and selectedElements.empty())
        {
            m_SelectedElement = 0;
            m_EventQueue.Push(Events::Gui::ShowProperties2 {0});
        }
    }
}