#include "pch.hpp"
#include "HandToolController.hpp"
#include "Input.hpp"

void HandToolController::OnUpdate()
{
	if (Input::IsMouseButtonDown(Mouse::ButtonLeft))
	{
		m_Camera.MoveOnScreenBy(Input::GetMouseDelta());
	}
}