#include "pch.hpp"
#include "Canvas/Controllers/CommonCanvasEntityController.hpp"
#include "Input.hpp"
#include "Events/CanvasEvents.hpp"

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
	if (isFocused && Input::IsKeyPressed(Key::Delete))
	{
		entity.Destroy();
		m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
	}
}