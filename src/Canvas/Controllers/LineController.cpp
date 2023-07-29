#include "pch.hpp"
#include "LineController.hpp"
#include "Input.hpp"
#include "Prefabs/ArrowEntity.hpp"
#include "Canvas/Canvas.hpp"

void LineController::OnUpdate()
{
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		ArrowEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition()), m_EventQueue).Build();
		LOG_DEBUG("Created ArrowEntity");
	}
}