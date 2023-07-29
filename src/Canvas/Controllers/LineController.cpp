#include "pch.hpp"
#include "LineController.hpp"
#include "Input.hpp"
#include "Prefabs/ArrowEntity.hpp"
#include "Canvas/Canvas.hpp"
#include "Events/CanvasEvents.hpp"

void LineController::OnUpdate()
{
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		auto entity = ArrowEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition()), m_EventQueue).Build();
		m_EventQueue.Push(Events::Canvas::SetFocus{ entity });
		LOG_DEBUG("Created ArrowEntity");
	}
}