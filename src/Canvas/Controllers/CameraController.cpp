#include "pch.hpp"
#include "CameraController.hpp"
#include "Render/Renderer.hpp"
#include "Input.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Canvas.hpp"


namespace
{
	void AdjustFilterToZoomLevel(float zoom, const Components::Image& sprite)
	{
		if (zoom < 1.0f)
			Renderer::SetImageFilter(*sprite.TextureId, ImageFilter::Linear);
		else
			Renderer::SetImageFilter(*sprite.TextureId, ImageFilter::Nearest);
	}
}


void CameraController::OnUpdate()
{
	glm::vec2 wheelMove = Input::GetMouseWheelMove();

	// Camera zoom
	if( Input::IsKeyDown( Key::LeftControl ) && wheelMove.y != 0 )
	{
		ZoomCamera( wheelMove.y );
	}
	else // Camera move
	{
		float zoomFactor = m_Camera.GetZoom();
		zoomFactor = zoomFactor < 1.0 ? 1.0f : zoomFactor;

		if( wheelMove.y != 0 )
		{
			glm::vec2 moveDelta = { 0.0f, wheelMove.y * 30.0f * zoomFactor };
			m_Camera.MoveOnScreenBy( moveDelta );
		}
		if( wheelMove.x != 0 )
		{
			glm::vec2 moveDelta = { wheelMove.x * 30.0f * zoomFactor, 0.0f };
			m_Camera.MoveOnScreenBy( moveDelta );
		}
	}

	// Aerial view
	if (Input::IsKeyPressed(Key::Space))
	{
		m_ZoomAnimation = Animation(m_Camera.GetZoom(), 0.25f, 0.5f, Animation::EasingType::ExpoOut);
	}
	if (Input::IsKeyDown(Key::Space))
	{
		float newZoom = m_ZoomAnimation.Step();
		m_Camera.SetZoom(newZoom);
	}
	if (Input::IsKeyReleased(Key::Space))
	{
		m_ZoomAnimation = Animation(m_Camera.GetZoom(), m_ZoomBeforeAnimation, 0.35f, Animation::EasingType::ExpoIn);

		glm::vec2 cameraStartPos = m_Camera.GetCenter();
		glm::vec2 cameraStopPos = Input::GetWorldMousePosition();
		m_CameraTargetAnimation = AnimationVec2(cameraStartPos, cameraStopPos, 0.35f, Animation::EasingType::ExpoOut);
	}
	if (Input::IsKeyUp(Key::Space) && !m_ZoomAnimation.Finished() || !m_CameraTargetAnimation.Finished())
	{
		float newZoom = m_ZoomAnimation.Step();
		m_Camera.SetZoom(newZoom);

		glm::vec2 newCameraCenter = m_CameraTargetAnimation.Step();
		m_Camera.CenterAtWorld(newCameraCenter);
	}

	if (Input::IsMouseButtonDown(Mouse::ButtonMiddle))
	{
		m_Camera.MoveOnScreenBy(Input::GetMouseDelta());
	}
}

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

	m_ZoomBeforeAnimation = zoomLevel;
	m_Camera.SetZoomAt(Input::GetScreenMousePosition(), zoomLevel);

	for (auto [entity, texture] : Canvas::GetAllEntitiesWith<Components::Image>().each())
	{
		AdjustFilterToZoomLevel(zoomLevel, texture);
	}
}
