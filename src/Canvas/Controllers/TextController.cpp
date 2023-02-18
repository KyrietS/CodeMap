#include "pch.hpp"
#include "TextController.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"
#include "Prefabs/TextEntity.hpp"


void TextController::OnUpdate()
{
	if (Input::IsMouseButtonDoubleClicked(Mouse::ButtonLeft))
	{
		TextEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition())).Build("Some text");
	}
}

