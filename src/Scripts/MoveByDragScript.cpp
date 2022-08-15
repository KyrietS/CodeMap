#include "pch.hpp"
#include "MoveByDragScript.hpp"
#include "Canvas/Components.hpp"
#include "Input.hpp"


void MoveByDragScript::OnCreate()
{
	assert(m_Entity.HasComponent<Components::Transform>());
	assert(m_Entity.HasComponent<Components::Focusable>());
}


void MoveByDragScript::OnUpdate()
{
	auto& transform = GetComponent<const Components::Transform>();
	auto& focus = GetComponent<const Components::Focusable>();
	auto isFocused = focus.IsFocused;
	Vector2 mouseWorldPos = Input::GetWorldMousePosition();
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isFocused
		&& (m_CurrentlyMoving || CheckCollisionPointRec(mouseWorldPos, focus.AsRectangle(transform))))
	{
		m_CurrentlyMoving = true;
		// Scale screen distance to world distance
		Vector2 delta = Vector2Scale(GetMouseDelta(), 1.0f / Canvas::Camera().GetZoom());
		MoveBy(delta);
	}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		m_CurrentlyMoving = false;
	}
}

void MoveByDragScript::MoveBy(Vector2 positionChange)
{
	auto& transform = GetComponent<Components::Transform>();
	transform.Translation = Vector2Add(transform.Translation, positionChange);
}
