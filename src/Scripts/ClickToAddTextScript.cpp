#include "pch.hpp"
#include "ClickToAddTextScript.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"
#include "Prefabs/TextEntity.hpp"


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
			TextEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition())).Build("Some text");
		}

	}
}

