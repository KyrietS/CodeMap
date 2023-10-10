#include "pch.hpp"
#include "SelectionController.hpp"
#include "Input.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Entity.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/GuiEvents.hpp"
#include "Canvas/Entity.hpp"


std::list<Entity> GetEntitiesUnderMouse()
{
	std::list<Entity> entitiesUnderMouse;
	auto view = Canvas::GetAllEntitiesWith<Components::Transform>();
	for (const Entity entity : view | std::views::reverse)
	{
		if (not entity.HasComponent<Components::Focusable>())
			continue;
		auto& transform = entity.GetComponent<Components::Transform>();
		auto& focusable = entity.GetComponent<Components::Focusable>();
		glm::vec2 vecWorldPos = Input::GetWorldMousePosition();
		if (focusable.AsBox(transform).Contains(vecWorldPos))
		{
			entitiesUnderMouse.push_back(entity);
		}
	}

	return entitiesUnderMouse;
}

void SelectionController::OnUpdate()
{
	// Dirty hack to allow HighlightController to work properly.
	// Better solution would be to make some Lock/Unlock Focus events.
	// TODO: Fix it when tools modes are implemented.
	if (Input::IsKeyDown(Key::LeftControl))
		return;

	if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
	{
		std::list<Entity> entitiesUnderMouse = GetEntitiesUnderMouse();

		// Clicked on empty space. Reset focus.
		if (entitiesUnderMouse.empty())
		{
			UnfocusAllEntities();
			return;
		}

		// Clicked on entities without focus. Set focus to the most upper one.
		bool noneEntityIsFocued = std::none_of(entitiesUnderMouse.begin(), entitiesUnderMouse.end(), [](Entity e) {
			return e.GetComponent<Components::Focusable>().IsFocused;
		});
		if (noneEntityIsFocued)
		{
			UnfocusAllEntities();
			FocusOn(entitiesUnderMouse.front());
			return;
		}
	}

	if (Input::IsMouseButtonClicked(Mouse::ButtonLeft))
	{
		std::list<Entity> entitiesUnderMouse = GetEntitiesUnderMouse();
		if (entitiesUnderMouse.size() < 2)
			return;

		// Clicked on entity with focus. Switch focus to entity under the focused one.
		// Except for text entities. They handle double click themselves and should not lose focus.
		auto focusedEntityUnderMouse = std::find_if(entitiesUnderMouse.begin(), entitiesUnderMouse.end(), [](Entity e) {
			return e.GetComponent<Components::Focusable>().IsFocused;
		});
		if (focusedEntityUnderMouse != entitiesUnderMouse.end() 
			&& !focusedEntityUnderMouse->HasComponent<Components::Text>())
		{
			auto nextEntity = std::next(focusedEntityUnderMouse);
			if (nextEntity == entitiesUnderMouse.end())
				nextEntity = entitiesUnderMouse.begin();
			FocusOn(*nextEntity);
			UnfocusOn(*focusedEntityUnderMouse);
		}
	}
}

void SelectionController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Canvas::SetFocus>(BIND_EVENT(SelectionController::OnSetFocus));
}

void SelectionController::OnSetFocus(const Events::Canvas::SetFocus& event)
{
	UnfocusAllEntities();
	FocusOn(entt::entity{ event.EntityId });
}

void SelectionController::FocusOn(Entity entity)
{
	if (entity && entity.HasComponent<Components::Focusable>())
	{
		entity.GetComponent<Components::Focusable>().IsFocused = true;
		m_EventQueue.Push(Events::Gui::ShowProperties{entity});
	}
	else
	{
		LOG_WARN("Tried to set focus to invalid entity");

	}
}

void SelectionController::UnfocusOn(Entity entity)
{
	if (entity && entity.HasComponent<Components::Focusable>())
	{
		entity.GetComponent<Components::Focusable>().IsFocused = false;
		m_EventQueue.Push(Events::Gui::ShowProperties(entt::entity{ entt::null }));
	}
	else
	{
		LOG_WARN("Tried to unset focus to invalid entity");
	}
}

void SelectionController::UnfocusAllEntities()
{
	for (const Entity& entity : Canvas::GetAllEntitiesWith<Components::Focusable>())
	{
		entity.GetComponent<Components::Focusable>().IsFocused = false;
	}
	m_EventQueue.Push(Events::Gui::ShowProperties(entt::entity{ entt::null }));
}
