#include "pch.hpp"
#include "LineController.hpp"
#include "Input.hpp"
#include "Prefabs/LineEntity.hpp"
#include "Canvas/Canvas.hpp"

void LineController::OnUpdate()
{
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		LineEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition())).Build();
		LOG_DEBUG("Created LineEntity");
	}
}