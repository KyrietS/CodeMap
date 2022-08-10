#include "pch.hpp"
#include "ClickToAddText.hpp"
#include "Canvas.hpp"
#include "Components.hpp"


namespace Script
{
	void ClickToAddText::OnUpdate()
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			Vector2 textPos = GetScreenToWorld2D(GetMousePosition(), Canvas::Camera());
			Entity textEntity = Canvas::Get().CreateEntity(textPos);
			auto& text = textEntity.AddComponent<Components::Text>("Some Text", 20.0f, BLUE);
		}
	}
}
