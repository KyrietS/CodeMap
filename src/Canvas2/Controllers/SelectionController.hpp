#pragma once
#include "IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/EventQueue.hpp"

namespace Controllers
{
class SelectionController : public IController
{
public:
    SelectionController(CanvasCamera& camera, EventQueue& eventQueue, CanvasElements& elements)
        : m_Camera(camera), m_EventQueue(eventQueue), m_Elements(elements) {}
    void Draw() override;

private:
    std::vector<ElementId> m_SelectedElements;

    CanvasCamera& m_Camera;
    EventQueue& m_EventQueue;
    CanvasElements& m_Elements;
};

}
