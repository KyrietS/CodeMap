#include "pch.hpp"
#include "CanvasViewControlScript.hpp"
#include "Input.hpp"


void CanvasViewControlScript::OnUpdate()
{
	Vector2 wheelMove = Input::GetMouseWheelMove();
	if (wheelMove.y != 0)
	{
		ZoomCamera(wheelMove.y);
	}
	if (wheelMove.x != 0)
	{
		Vector2 moveDelta = { wheelMove.x * 30.0f, 0.0f };
		m_Camera.MoveOnScreenBy(moveDelta);
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

		Vector2 cameraStartPos = m_Camera.GetCenter();
		Vector2 cameraStopPos = Input::GetWorldMousePosition();
		m_CameraTargetAnimation = AnimationVec2(cameraStartPos, cameraStopPos, 0.35f, Animation::EasingType::ExpoOut);
	}
	if (Input::IsKeyUp(Key::Space) && !m_ZoomAnimation.Finished() || !m_CameraTargetAnimation.Finished())
	{
		float newZoom = m_ZoomAnimation.Step();
		m_Camera.SetZoom(newZoom);
		
		Vector2 newCameraCenter = m_CameraTargetAnimation.Step();
		m_Camera.CenterAtWorld(newCameraCenter);
	}

	if (Input::IsMouseButtonDown(Mouse::ButtonMiddle))
	{
		m_Camera.MoveOnScreenBy(Input::GetMouseDelta());
	}
}

void CanvasViewControlScript::ZoomCamera(float zoomChange)
{
	const float zoomFactor = 1.1f;
	float zoomLevel = m_Camera.GetZoom() * (zoomChange > 0 ? zoomFactor : (1.0f / zoomFactor));

	// Limits
	zoomLevel = Clamp(zoomLevel, 0.1f, 10.0f);

	// Snap to 1.0
	if (zoomLevel > 0.95f && zoomLevel < 1.05f)
	{
		zoomLevel = 1.0f;
	}

	m_ZoomBeforeAnimation = zoomLevel;
	m_Camera.SetZoomAt(Input::GetScreenMousePosition(), zoomLevel);

	for (auto [entity, texture] : Canvas::GetAllEntitiesWith<Components::Sprite>().each())
	{
		AdjustFilterToZoomLevel(zoomLevel, texture);
	}
}

void CanvasViewControlScript::AdjustFilterToZoomLevel(float zoom, Texture2D& texture)
{
	if (zoom < 1.0f)
		SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
	else
		SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}
