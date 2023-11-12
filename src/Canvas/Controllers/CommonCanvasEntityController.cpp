#include "pch.hpp"
#include "Canvas/Controllers/CommonCanvasEntityController.hpp"
#include "Input.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas/Canvas.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/GuiEvents.hpp"

void CommonCanvasEntityController::OnUpdate()
{
	auto entities = Canvas::GetAllEntitiesWith<Components::Transform, Components::Focusable>();
	for (Entity entity : entities)
	{
		OnUpdate(entity);
	}
}

void CommonCanvasEntityController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Canvas::SetFocus>(BIND_EVENT(CommonCanvasEntityController::OnSetFocus));
	dispatcher.Dispatch<Events::Canvas::ClearFocus>(BIND_EVENT(CommonCanvasEntityController::OnClearFocus));
}

void CommonCanvasEntityController::OnSetFocus(const Events::Canvas::SetFocus& event)
{
	UnfocusAllEntities();
	FocusOn(entt::entity{ event.EntityId });
}

void CommonCanvasEntityController::OnClearFocus(const Events::Canvas::ClearFocus&)
{
	UnfocusAllEntities();
}

void CommonCanvasEntityController::FocusOn(Entity entity)
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

void CommonCanvasEntityController::UnfocusAllEntities()
{
	for (const Entity& entity : Canvas::GetAllEntitiesWith<Components::Focusable>())
	{
		entity.GetComponent<Components::Focusable>().IsFocused = false;
	}
	m_EventQueue.Push(Events::Gui::ShowProperties(entt::entity{ entt::null }));
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