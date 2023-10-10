#include "pch.hpp"
#include "HighlightController.hpp"
#include "Input.hpp"
#include "Events/CanvasEvents.hpp"
#include "Prefabs/HighlightEntity.hpp"
#include "Canvas/Canvas.hpp"

bool noHighlightIsFocused()
{
	auto entities = Canvas::GetAllEntitiesWith<Components::Highlight>();
	return std::none_of(entities.begin(), entities.end(), [](Entity entity) {
		return entity.GetComponent<Components::Focusable>().IsFocused;
	});
}

void HighlightController::OnUpdate()
{
	if (Input::IsKeyDown(Key::LeftControl) && Input::IsMouseButtonPressed(Mouse::ButtonLeft)
		&& noHighlightIsFocused())
	{
		auto entity = HighlightEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition()), m_EventQueue);
		const auto& transform = entity.GetComponent<Components::Transform>();
		entity.GetComponent<Components::Highlight>().AddPoint(transform, Input::GetWorldMousePosition());
		m_EventQueue.Push(Events::Canvas::SetFocus{ entity });
		LOG_DEBUG("Created Highlight entity");
	}
}