#include "pch.hpp"
#include "SelectionController.hpp"
#include "Render/VColor.hpp"
#include "Render/Renderer.hpp"
#include "Canvas/CanvasCamera.hpp"


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
}