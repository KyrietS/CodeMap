#include "pch.hpp"
#include "Canvas/Controllers/CommonCanvasEntityController.hpp"
#include "Input.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas/Canvas.hpp"

void CommonCanvasEntityController::OnUpdate()
{
	auto entities = Canvas::GetAllEntitiesWith<Components::Transform, Components::Focusable>();
	for (Entity entity : entities)
	{
		OnUpdate(entity);
	}
}

void CommonCanvasEntityController::OnUpdate(Entity entity)
{
	auto isFocused = entity.GetComponent<Components::Focusable>().IsFocused;

	if (isFocused && Input::IsKeyPressed(Key::PageUp, true))
	{
		entity.GetComponent<Components::Transform>().Index += 2;
		m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
	}
	if (isFocused && Input::IsKeyPressed(Key::PageDown, true))
	{
		entity.GetComponent<Components::Transform>().Index -= 2;
		m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
	}
	// TODO: Make it more extensible because now Highlight component
	//       need to handle deleting by itself because only this
	//       component knows if control point is selected
	if (isFocused && Input::IsKeyPressed(Key::Delete) && !entity.HasComponent<Components::Highlight>())
	{
		entity.Destroy();
		m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
	}
}