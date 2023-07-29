#include "pch.hpp"
#include "TextController.hpp"
#include "Canvas/Canvas.hpp"
#include "Input.hpp"
#include "Prefabs/TextEntity.hpp"
#include "Events/CanvasEvents.hpp"


void TextController::OnUpdate()
{
	if (Input::IsMouseButtonDoubleClicked(Mouse::ButtonLeft))
	{
		const int FONT_SIZE = 32;
		TextEntity(Canvas::Get().CreateEntity(Input::GetWorldMousePosition()), m_EventQueue).Build("Some text", FONT_SIZE);
		m_EventQueue.Push(Events::Canvas::MakeSnapshot{});
	}
}

