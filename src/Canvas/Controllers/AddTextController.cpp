#include "pch.hpp"
#include "AddTextController.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"
#include "Prefabs/TextEntity.hpp"


void AddTextController::OnUpdate()
{
	if (Input::IsMouseButtonDoubleClicked(Mouse::ButtonLeft))
	{
		TextEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition())).Build("Some text");
	}
}

