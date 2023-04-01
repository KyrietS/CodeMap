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

	if (not focus.IsDraggable)
		return;

	auto isFocused = focus.IsFocused;
	glm::vec2 mouseVecPos = Input::GetWorldMousePosition();
	if (Input::IsMouseButtonDown(Mouse::ButtonLeft) && isFocused
		&& (m_CurrentlyMoving || focus.AsBox(transform).Contains(mouseVecPos)))
	{
		m_CurrentlyMoving = true;
		// Scale screen distance to world distance
		glm::vec2 delta = Input::GetMouseDelta() * (1.0f / Canvas::Camera().GetZoom());
		MoveBy(delta);
	}

	if (Input::IsMouseButtonReleased(Mouse::ButtonLeft))
	{
		m_CurrentlyMoving = false;
	}
}

void MoveByDragScript::MoveBy(glm::vec2 positionChange)
{
	glm::vec2 posChange = { positionChange.x, positionChange.y };
	auto& transform = GetComponent<Components::Transform>();
	//transform.Translation = Vector2Add(transform.Translation, positionChange);
	transform.Translation += posChange;
}
