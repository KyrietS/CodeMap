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
		glm::vec2 vecWorldPos = Input::GetWorldMousePosition(Canvas::Camera());
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
			m_EventQueue.Push(Events::Canvas::ClearFocus{});
			return;
		}

		// Clicked on entities without focus. Set focus to the most upper one.
		bool noneEntityIsFocued = std::none_of(entitiesUnderMouse.begin(), entitiesUnderMouse.end(), [](Entity e) {
			return e.GetComponent<Components::Focusable>().IsFocused;
		});
		if (noneEntityIsFocued)
		{
			m_EventQueue.Push(Events::Canvas::SetFocus{entitiesUnderMouse.front()});
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
			m_EventQueue.Push(Events::Canvas::SetFocus{*nextEntity});
		}
	}
}
