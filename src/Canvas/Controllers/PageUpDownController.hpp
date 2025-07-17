#pragma once
#include "Canvas/CanvasElements.hpp"
#include "Events/EventQueue.hpp"
#include "IController.hpp"
#include "KeyCodes.hpp"

namespace Events::Input
{
    class KeyPressed;
}

namespace Controllers
{
class PageUpDownController : public IController
{
public:
    PageUpDownController(EventQueue& eventQueue, CanvasElements& elements)
        : m_EventQueue(eventQueue), m_Elements(elements) {}
    void OnEvent(Event&) override;

private:
	bool OnKeyPressed(const Events::Input::KeyPressed&);
    bool HandleKeyPressed(KeyCode);
    void MoveElementsUp();
    void MoveElementsDown();
    std::list<ElementId> GetSelectedElements();

    EventQueue& m_EventQueue;
    CanvasElements& m_Elements;
};

}
