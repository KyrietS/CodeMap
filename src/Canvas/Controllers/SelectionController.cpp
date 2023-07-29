#include "pch.hpp"
#include "SelectionController.hpp"
#include "Input.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
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

void UnfocusAllEntities()
{
	for (Entity e : Canvas::GetAllEntitiesWith<Components::Focusable>())
	{
		e.GetComponent<Components::Focusable>().IsFocused = false;
	}
}

void SelectionController::OnUpdate()
{
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
			entitiesUnderMouse.front().GetComponent<Components::Focusable>().IsFocused = true;
			return;
		}
	}


	if (Input::IsMouseButtonClicked(Mouse::ButtonLeft))
	{
		std::list<Entity> entitiesUnderMouse = GetEntitiesUnderMouse();
		if (entitiesUnderMouse.size() < 2)
			return;

		// Clicked on entity with focus. Switch focus to entity under the focused one.
		auto focusedEntityUnderMouse = std::find_if(entitiesUnderMouse.begin(), entitiesUnderMouse.end(), [](Entity e) {
			return e.GetComponent<Components::Focusable>().IsFocused;
		});
		if (focusedEntityUnderMouse != entitiesUnderMouse.end())
		{
			auto nextEntity = std::next(focusedEntityUnderMouse);
			if (nextEntity == entitiesUnderMouse.end())
				nextEntity = entitiesUnderMouse.begin();
			(*nextEntity).GetComponent<Components::Focusable>().IsFocused = true;
			(*focusedEntityUnderMouse).GetComponent<Components::Focusable>().IsFocused = false;
		}
	}
}