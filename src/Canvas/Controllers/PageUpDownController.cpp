#include "PageUpDownController.hpp"

#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/KeyEvents.hpp"
#include "Logger.hpp"

namespace Controllers
{
void PageUpDownController::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(OnKeyPressed));
}

bool PageUpDownController::OnKeyPressed(const Events::Input::KeyPressed& event)
{
    if (HandleKeyPressed(event.GetKey()))
    {
        m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
        return true;
    }

    return false;
}
bool PageUpDownController::HandleKeyPressed(const KeyCode key)
{
    if (key == Key::PageUp)
    {
        LOG_DEBUG("PageUp: Moving selected elements up");
        MoveElementsUp();
        return true;
    }
    if (key == Key::PageDown)
    {
        LOG_DEBUG("PageDown: Moving selected elements down");
        MoveElementsDown();
        return true;
    }

    return false;
}

void PageUpDownController::MoveElementsUp()
{
    for (const ElementId id : GetSelectedElements())
    {
        m_Elements.Move(id, -1);
    }
}

void PageUpDownController::MoveElementsDown()
{
    auto selectedElements = GetSelectedElements();
    selectedElements.reverse();
    for (const ElementId id : selectedElements)
    {
        m_Elements.Move(id, 1);
    }
}

std::list<ElementId> PageUpDownController::GetSelectedElements()
{
    std::list<ElementId> selectedElements;
    for (const auto& [id, element] : m_Elements)
    {
        if (element->InEditMode)
        {
            selectedElements.push_back(id);
        }
    }
    return selectedElements;
}

} // namespace Controllers
