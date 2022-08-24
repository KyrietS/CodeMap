#include "pch.hpp"
#include "CommonCanvasEntityScript.hpp"
#include "Canvas/Components.hpp"
#include "Input.hpp"


void CommonCanvasEntityScript::OnUpdate()
{
	auto isFocused = GetComponent<Components::Focusable>().IsFocused;

	if (isFocused && Input::IsKeyPressed(Key::PageUp, true))
	{
		GetComponent<Components::Transform>().Index += 2;
	}
	if (isFocused && Input::IsKeyPressed(Key::PageDown, true))
	{
		GetComponent<Components::Transform>().Index -= 2;
	}
	if (isFocused && Input::IsKeyPressed(Key::Delete))
	{
		m_Entity.Destroy();
	}
}
