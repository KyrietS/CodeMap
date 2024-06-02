#pragma once
#include "IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas/CanvasCamera.hpp"

namespace Controllers
{
class SelectionController : public IController
{
public:
    SelectionController(CanvasCamera& camera, CanvasElements& elements)
        : m_Camera(camera), m_Elements(elements) {}
    void Draw() override;

private:
    CanvasCamera& m_Camera;
    CanvasElements& m_Elements;
};

}
