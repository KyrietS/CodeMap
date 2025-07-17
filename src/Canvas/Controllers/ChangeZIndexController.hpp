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
class ChangeZIndexController : public IController
{
public:
    ChangeZIndexController(EventQueue& eventQueue, CanvasElements& elements)
        : m_EventQueue(eventQueue), m_Elements(elements) {}
    void OnEvent(Event&) override;

private:
	bool OnKeyPressed(const Events::Input::KeyPressed&);
    bool HandleKeyPressed(KeyCode);
    void MoveElementsUp(int count = 1);
    void MoveElementsDown(int count = 1);
    void BringElementsToFront();
    void SendElementsToBack();
    std::list<ElementId> GetSelectedElements();

    EventQueue& m_EventQueue;
    CanvasElements& m_Elements;
};

}
