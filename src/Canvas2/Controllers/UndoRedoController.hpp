#pragma once
#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas2/CanvasElements.hpp"

namespace Controllers
{
class UndoRedoController : public IController
{
public:
    UndoRedoController(EventQueue& eventQueue, CanvasElements& elements) 
        : m_EventQueue(eventQueue), m_Elements(elements) {}
    void OnEvent(Event& event) override;

private:
    void OnPushSnapshotEvent(const Events::Canvas::MakeSnapshot& event);
    void OnUndoEvent(const Events::Canvas::Undo& event);
    void OnRedoEvent(const Events::Canvas::Redo& event);

    CanvasElements CloneElements() const;

    EventQueue& m_EventQueue;
    CanvasElements& m_Elements;

    CanvasElements m_CurrentSnapshot;
    std::stack<CanvasElements> m_Snapshots;
    std::stack<CanvasElements> m_RedoSnapshots;
};

}
