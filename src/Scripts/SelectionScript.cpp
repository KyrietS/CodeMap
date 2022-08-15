#include "pch.hpp"
#include "SelectionScript.hpp"
#include "Input.hpp"

void SelectionScript::OnUpdate()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		// reset focus
		for (auto [entity, focusable] : Canvas::GetAllEntitiesWith<Components::Focusable>().each())
		{
			focusable.IsFocused = false;
		}

		auto view = Canvas::GetAllEntitiesWith<Components::Transform>();
		for (const Entity entity : view | std::views::reverse)
		{
			if (!entity.HasComponent<Components::Focusable>())
				continue;
			auto& transform = entity.GetComponent<Components::Transform>();
			auto& focusable = entity.GetComponent<Components::Focusable>();
			Vector2 worldPos = Input::GetWorldMousePosition();
			// TODO: Rotation is not taken into account!
			if (CheckCollisionPointRec(worldPos, focusable.AsRectangle(transform)))
			{
				focusable.IsFocused = true;
				break;
			}
		}
	}
}