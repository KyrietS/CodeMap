#pragma once

#include "ScriptableEntity.hpp"
#include "Components.hpp"


struct SelectionScript : ScriptableEntity
{
	void OnUpdate() override
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			// reset focus
			for (auto [entity, focusable] : Canvas::Get().GetComponents<Components::Focusable>().each())
			{
				focusable.IsFocused = false;
			}

			auto view = Canvas::Get().GetComponents<Components::Transform>();
			for (auto entity : view | std::views::reverse)
			{
				if (!Canvas::Get().HasComponents<Components::Focusable>(entity))
					continue;
				auto& focusable = Canvas::Get().GetComponent<Components::Focusable>(entity);
				Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), Canvas::Camera());
				if (CheckCollisionPointRec(worldPos, focusable.FocusArea))
				{
					focusable.IsFocused = true;
					break;
				}
			}
		}
	}
};