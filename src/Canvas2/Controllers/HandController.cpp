#include "pch.hpp"
#include "HandController.hpp"
#include "Events/EventDispatcher.hpp"
#include "Input.hpp"

namespace Controllers
{
void Controllers::HandController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Handle<Events::Input::MousePressed>(BIND_EVENT(HandController::OnMousePressed));
	dispatcher.Handle<Events::Input::MouseReleased>(BIND_EVENT(HandController::OnMouseReleased));
	dispatcher.Handle<Events::Input::MouseMoved>(BIND_EVENT(HandController::OnMouseMoved));
}

bool HandController::OnMousePressed(const Events::Input::MousePressed& event)
{
	if (event.GetButton() == Mouse::ButtonLeft)
	{
		m_IsCameraDragging = true;
		return true;
	}
	m_IsCameraDragging = false;
	return false;
}

bool HandController::OnMouseReleased(const Events::Input::MouseReleased& event)
{
	if (event.GetButton() == Mouse::ButtonLeft)
	{
		m_IsCameraDragging = false;
		return true;
	}
	return false;
}

bool HandController::OnMouseMoved(const Events::Input::MouseMoved& event)
{
	if (m_IsCameraDragging && Input::IsMouseDragged(Mouse::ButtonLeft))
	{
		m_Camera.MoveOnScreenBy(Input::GetMouseDelta());
		return true;
	}
	return false;
}

}
