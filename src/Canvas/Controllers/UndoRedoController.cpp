#include "pch.hpp"
#include "Canvas/Controllers/UndoRedoController.hpp"
#include "Canvas/Entity.hpp"
#include "Input.hpp"
#include "Events/EventDispatcher.hpp"

void UndoRedoController::OnUpdate()
{
}

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
    m_Snapshots.push(std::move(m_CurrentSnapshot));
    m_CurrentSnapshot = CloneRegistry();
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
        m_Registry.clear();
		return;
	}

    auto& snapshot = m_Snapshots.top();
    m_Registry = std::move(snapshot);
    m_Snapshots.pop();
    m_RedoSnapshots.push(std::move(m_CurrentSnapshot));
    m_CurrentSnapshot = CloneRegistry();
}

void UndoRedoController::OnRedoEvent(const Events::Canvas::Redo& event)
{
    LOG_INFO("Redo event received");
    if (m_RedoSnapshots.empty())
    {
		return;
    }

    auto& snapshot = m_RedoSnapshots.top();
    m_Registry = std::move(snapshot);
    m_RedoSnapshots.pop();
    m_Snapshots.push(std::move(m_CurrentSnapshot));
    m_CurrentSnapshot = CloneRegistry();
}

template<typename Component>
void CopyComponentIfPresent(Entity srcEntity, entt::entity dstEntity, entt::registry& dstRegistry)
{
    if (srcEntity.HasComponent<Component>())
    {
        dstRegistry.emplace_or_replace<Component>(dstEntity, srcEntity.GetComponent<Component>());
	}
}

entt::registry UndoRedoController::CloneRegistry() const
{
    entt::registry registry;
    for (Entity entity : m_Registry.view<Components::Transform>())
    {
        entt::entity newEntity = registry.create();

        CopyComponentIfPresent<Components::Transform>(entity, newEntity, registry);
        CopyComponentIfPresent<Components::Hierarchy>(entity, newEntity, registry);
        CopyComponentIfPresent<Components::Image>(entity, newEntity, registry);
        CopyComponentIfPresent<Components::Arrow>(entity, newEntity, registry);
        CopyComponentIfPresent<Components::Text>(entity, newEntity, registry);
        CopyComponentIfPresent<Components::NativeScript>(entity, newEntity, registry);
        CopyComponentIfPresent<Components::Focusable>(entity, newEntity, registry);
	}

    return registry;
}