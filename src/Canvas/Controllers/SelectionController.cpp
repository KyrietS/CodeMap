#include "pch.hpp"
#include "SelectionController.hpp"
#include "Input.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Entity.hpp"


void SelectionController::OnUpdate()
{
	if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
	{
		// reset focus
		for (auto [entity, focusable] : Canvas::GetAllEntitiesWith<Components::Focusable>().each())
		{
			focusable.IsFocused = false;
		}

		auto view = Canvas::GetAllEntitiesWith<Components::Transform>();
		for (const Entity entity : view | std::views::reverse)
		{
			if (not entity.HasComponent<Components::Focusable>())
				continue;
			auto& transform = entity.GetComponent<Components::Transform>();
			auto& focusable = entity.GetComponent<Components::Focusable>();
			glm::vec2 vecWorldPos = Input::GetWorldMousePosition();
			// TODO: Rotation is not taken into account!
			if (focusable.AsBox(transform).Contains(vecWorldPos))
			{
				focusable.IsFocused = true;
				break;
			}
		}
	}
}