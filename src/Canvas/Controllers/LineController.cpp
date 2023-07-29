#include "pch.hpp"
#include "LineController.hpp"
#include "Input.hpp"
#include "Prefabs/ArrowEntity.hpp"
#include "Canvas/Canvas.hpp"

void LineController::OnUpdate()
{
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		auto entity = ArrowEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition()), m_EventQueue).Build();
		// Send event to focus this entity and unfocus all others
		entity.GetComponent<Components::Focusable>().IsFocused = true;
		LOG_DEBUG("Created ArrowEntity");
	}
}