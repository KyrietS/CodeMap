#include "pch.hpp"
#include "CommonCanvasEntityScript.hpp"
#include "Canvas/Components.hpp"


void CommonCanvasEntityScript::OnUpdate()
{
	auto isFocused = GetComponent<Components::Focusable>().IsFocused;

	if (isFocused && IsKeyPressed(KEY_PAGE_UP))
	{
		GetComponent<Components::Transform>().Index += 2;
	}
	if (isFocused && IsKeyPressed(KEY_PAGE_DOWN))
	{
		GetComponent<Components::Transform>().Index -= 2;
	}
	if (isFocused && IsKeyPressed(KEY_DELETE))
	{
		m_Entity.Destroy();
	}
}
