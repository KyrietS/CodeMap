#include "pch.hpp"
#include "Canvas/Controllers/UndoRedoController.hpp"
#include "Input.hpp"
#include "Events/EventDispatcher.hpp"
#include "Canvas/Elements/ArrowElement.hpp"
#include "Canvas/Elements/ShapeElement.hpp"
#include "Canvas/Elements/TextElement.hpp"
#include "Canvas/Elements/ImageElement.hpp"
#include <ranges>

namespace Controllers
{

    void UndoRedoController::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Events::Canvas::MakeSnapshot>(BIND_EVENT(UndoRedoController::OnPushSnapshotEvent));
        dispatcher.Dispatch<Events::Canvas::Undo>(BIND_EVENT(UndoRedoController::OnUndoEvent));
        dispatcher.Dispatch<Events::Canvas::Redo>(BIND_EVENT(UndoRedoController::OnRedoEvent));
    }

    void UndoRedoController::OnPushSnapshotEvent(const Events::Canvas::MakeSnapshot& event)
    {
        LOG_INFO("Push snapshot event received");
        // TODO: Don't make a snapshot if nothing has changed

        m_Snapshots.push(std::move(m_CurrentSnapshot));
        m_CurrentSnapshot = CloneElements();
        m_RedoSnapshots = {};
    }

    void UndoRedoController::OnUndoEvent(const Events::Canvas::Undo& event)
    {
        LOG_INFO("Undo event received");
        if (m_Snapshots.empty())
        {
            // TODO: What if canvas was loaded from file?
            //       Then I don't want to clear the canvas.
            //       I want to restore it to the state when
            //       it was loaded.
            m_Elements.Clear();
            return;
        }

        auto& snapshot = m_Snapshots.top();
        m_Elements = std::move(snapshot);
        m_Snapshots.pop();
        m_RedoSnapshots.push(std::move(m_CurrentSnapshot));
        m_CurrentSnapshot = CloneElements();
    }

    void UndoRedoController::OnRedoEvent(const Events::Canvas::Redo& event)
    {
        LOG_INFO("Redo event received");
        if (m_RedoSnapshots.empty())
        {
            return;
        }

        auto& snapshot = m_RedoSnapshots.top();
        m_Elements = std::move(snapshot);
        m_RedoSnapshots.pop();
        m_Snapshots.push(std::move(m_CurrentSnapshot));
        m_CurrentSnapshot = CloneElements();
    }

    template<typename ElementT>
    static void CopyElementIfTypeOf(const Elements::IElement& element, CanvasElements& elements)
    {
        if (const auto* concreteElement = element.As<const ElementT>())
        {
            auto clonedElement = std::make_unique<ElementT>(*concreteElement);
            elements.Add(std::move(clonedElement));
        }
    }

    CanvasElements UndoRedoController::CloneElements() const
    {
        // TODO: return elements.Clone();
        CanvasElements elements;
        for (const auto& [id, element] : m_Elements | std::views::reverse)
        {
            // TODO: Consider using IElement::Clone() method
            CopyElementIfTypeOf<Elements::ArrowElement>(*element, elements);
            CopyElementIfTypeOf<Elements::ShapeElement>(*element, elements);
            CopyElementIfTypeOf<Elements::TextElement>(*element, elements);
            CopyElementIfTypeOf<Elements::ImageElement>(*element, elements);
        }

        return elements;
    }
}