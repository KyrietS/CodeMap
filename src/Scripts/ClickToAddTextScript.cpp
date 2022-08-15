#include "pch.hpp"
#include "ClickToAddTextScript.hpp"
#include "Scripts/MoveByDragScript.hpp"
#include "Scripts/CommonCanvasEntityScript.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
#include "Input.hpp"


void ClickToAddTextScript::OnUpdate()
{
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		static auto before = std::chrono::system_clock::now();
		auto now = std::chrono::system_clock::now();
		double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
		before = now;

		// Double-click
		if (diff_ms > 10 && diff_ms < 200) {
			Vector2 textPos = Input::GetWorldMousePosition();
			Entity textEntity = Canvas::Get().CreateEntity(textPos);
			auto& text = textEntity.AddComponent<Components::Text>("Some Text", 20.0f, BLUE);

			auto& focus = textEntity.AddComponent<Components::Focusable>();
			focus.Size = MeasureTextEx(text.Font, text.Content.c_str(), text.Size, text.Spacing);
			focus.IsFocused = true;

			textEntity.AttachScript<MoveByDragScript>();
			textEntity.AttachScript<CommonCanvasEntityScript>();
		}

	}
}

