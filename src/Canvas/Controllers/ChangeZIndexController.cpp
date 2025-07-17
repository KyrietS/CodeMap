#include "ChangeZIndexController.hpp"

#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/KeyEvents.hpp"
#include "Logger.hpp"

namespace Controllers
{
void ChangeZIndexController::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Handle<Events::Input::KeyPressed>(BIND_EVENT(OnKeyPressed));
}

bool ChangeZIndexController::OnKeyPressed(const Events::Input::KeyPressed& event)
{
    if (HandleKeyPressed(event.GetKey()))
    {
        m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
        return true;
    }

    return false;
}
bool ChangeZIndexController::HandleKeyPressed(const KeyCode key)
{
    if (key == Key::PageUp)
    {
        LOG_DEBUG("PageUp: Move selected elements up");
        MoveElementsUp();
        return true;
    }
    if (key == Key::PageDown)
    {
        LOG_DEBUG("PageDown: Move selected elements down");
        MoveElementsDown();
        return true;
    }
    if (key == Key::Home)
    {
        LOG_DEBUG("Home: Bring selected elements to front");
        BringElementsToFront();
        return true;
    }
    if (key == Key::End)
    {
        LOG_DEBUG("End: Send selected elements to back");
        SendElementsToBack();
        return true;
    }

    return false;
}

void ChangeZIndexController::MoveElementsUp(const int count)
{
    for (const ElementId id : GetSelectedElements())
    {
        m_Elements.Move(id, -count);
    }
}

void ChangeZIndexController::MoveElementsDown(const int count)
{
    auto selectedElements = GetSelectedElements();
    selectedElements.reverse();
    for (const ElementId id : selectedElements)
    {
        m_Elements.Move(id, count);
    }
}

void ChangeZIndexController::BringElementsToFront()
{
    MoveElementsUp(999'999);
}

void ChangeZIndexController::SendElementsToBack()
{
    MoveElementsDown(999'999);
}

std::list<ElementId> ChangeZIndexController::GetSelectedElements()
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
