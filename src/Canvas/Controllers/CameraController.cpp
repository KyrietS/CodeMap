#include "pch.hpp"
#include "CameraController.hpp"
#include "Input.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/GuiEvents.hpp"
#include "Canvas/Elements/ImageElement.hpp"
#include "Render/Renderer.hpp"

namespace
{
	void AdjustFilterToZoomLevel(float zoom, const Elements::ImageElement& image)
	{
		if (zoom < 1.0f)
			Renderer::SetImageFilter(*image.GetData().TextureId, ImageFilter::Linear);
		else
			Renderer::SetImageFilter(*image.GetData().TextureId, ImageFilter::Nearest);
	}
}

namespace Controllers
{
	void CameraController::ZoomCamera(float zoomChange)
	{
		const float zoomFactor = 1.1f;
		float zoomLevel = m_Camera.GetZoom() * (zoomChange > 0 ? zoomFactor : (1.0f / zoomFactor));

		// Limits
		zoomLevel = glm::clamp(zoomLevel, 0.1f, 10.0f);

		// Snap to 1.0
		if (zoomLevel > 0.95f && zoomLevel < 1.05f)
		{
			zoomLevel = 1.0f;
		}

		m_Camera.SetZoomAt(Input::GetScreenMousePosition(), zoomLevel);

		for (auto& [id, element] : m_Elements)
		{
			if (auto* image = element->As<Elements::ImageElement>())
			{
				AdjustFilterToZoomLevel(zoomLevel, *image);
			}
		}

		m_EventQueue.Push(Events::Gui::ZoomChanged { zoomLevel });
	}

	void CameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Handle<Events::Input::MouseMoved>(BIND_EVENT(CameraController::OnMouseMoved));
		dispatcher.Dispatch<Events::Input::MouseScrolled>(BIND_EVENT(CameraController::OnMouseScrolled));
	}

	bool CameraController::OnMouseMoved(const Events::Input::MouseMoved& event)
	{
		if (Input::IsMouseDragged(Mouse::ButtonMiddle))
		{
			m_Camera.MoveOnScreenBy(Input::GetMouseDelta());
			return true;
		}
		return false;
	}

	void CameraController::OnMouseScrolled(const Events::Input::MouseScrolled& event)
	{
		glm::vec2 wheelMove = Input::GetMouseWheelMove();

		// Camera zoom
		if (Input::IsControlDown() && wheelMove.y != 0)
		{
			ZoomCamera(wheelMove.y);
		}
		else // Camera move
		{
			float zoomFactor = m_Camera.GetZoom();
			zoomFactor = zoomFactor < 1.0 ? 1.0f : zoomFactor;
			glm::vec2 moveDelta = wheelMove * 30.0f * zoomFactor;
			m_Camera.MoveOnScreenBy(moveDelta);
		}
	}
}