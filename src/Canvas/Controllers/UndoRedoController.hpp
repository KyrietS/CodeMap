#pragma once

#include "IController.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"
#include "entt/entt.hpp"

class UndoRedoController : public IController
{
public:
    UndoRedoController(EventQueue& eventQueue, entt::registry& registry) : m_EventQueue(eventQueue), m_Registry(registry) {}
    void OnUpdate() override;
    void OnEvent(Event& event) override;

private:
    void OnPushSnapshotEvent(const Events::Canvas::MakeSnapshot& event);
    void OnUndoEvent(const Events::Canvas::Undo& event);
    void OnRedoEvent(const Events::Canvas::Redo& event);

    entt::registry CloneRegistry() const;

    EventQueue& m_EventQueue;
    entt::registry& m_Registry;

    entt::registry m_CurrentSnapshot;
    std::stack<entt::registry> m_Snapshots;
    std::stack<entt::registry> m_RedoSnapshots;
};