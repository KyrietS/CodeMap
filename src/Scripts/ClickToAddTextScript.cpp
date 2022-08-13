#include "pch.hpp"
#include "ClickToAddTextScript.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
#include "Input.hpp"

void ClickToAddTextScript::OnUpdate()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		//Vector2 textPos = Canvas::Camera().GetScreenToWorld(GetMousePosition());
		Vector2 textPos = Input::GetWorldMousePosition();
		Entity textEntity = Canvas::Get().CreateEntity(textPos);
		auto& text = textEntity.AddComponent<Components::Text>("Some Text", 20.0f, BLUE);
	}
}

