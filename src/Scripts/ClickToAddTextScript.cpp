#include "pch.hpp"
#include "ClickToAddTextScript.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"
#include "Prefabs/TextEntity.hpp"


void ClickToAddTextScript::OnUpdate()
{
	if (Input::IsMouseButtonDoubleClicked(Mouse::ButtonLeft))
	{
		TextEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition())).Build("Some text");
	}
}

