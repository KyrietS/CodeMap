#include "pch.hpp"
#include "CanvasViewControlScript.hpp"
#include "Input.hpp"


void CanvasViewControlScript::OnUpdate()
{
	float zoomChange = GetMouseWheelMove();
	if (zoomChange != 0)
	{
		ZoomCamera(zoomChange);
	}

	// Aerial view
	if (IsKeyPressed(KEY_SPACE))
	{
		m_ZoomAnimation = Animation(m_Camera.GetZoom(), 0.25f, 0.5f, Animation::EasingType::ExpoOut);
	}
	if (IsKeyDown(KEY_SPACE))
	{
		float newZoom = m_ZoomAnimation.Step();
		m_Camera.SetZoom(newZoom);
	}
	if (IsKeyReleased(KEY_SPACE))
	{
		m_ZoomAnimation = Animation(m_Camera.GetZoom(), m_ZoomBeforeAnimation, 0.35f, Animation::EasingType::ExpoIn);

		Vector2 cameraStartPos = m_Camera.GetCenter();
		Vector2 cameraStopPos = Input::GetWorldMousePosition();
		m_CameraTargetAnimation = AnimationVec2(cameraStartPos, cameraStopPos, 0.35f, Animation::EasingType::ExpoOut);
	}
	if (IsKeyUp(KEY_SPACE) && !m_ZoomAnimation.Finished() || !m_CameraTargetAnimation.Finished())
	{
		float newZoom = m_ZoomAnimation.Step();
		m_Camera.SetZoom(newZoom);
		
		Vector2 newCameraCenter = m_CameraTargetAnimation.Step();
		m_Camera.CenterAtWorld(newCameraCenter);
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
	{
		m_Camera.MoveOnScreenBy(GetMouseDelta());
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
	m_Camera.SetZoomAt(GetMousePosition(), zoomLevel);

	for (auto [entity, texture] : Canvas::GetAllEntitiesWith<Components::Sprite>().each())
	{
		AdjustFilterToZoomLevel(zoomLevel, texture);
	}
}

//void CanvasViewControlScript::SetCameraZoomAt(float zoomLevel)
//{
//	Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), m_Camera);
//	m_Camera.offset = GetMousePosition();
//	m_Camera.target = mouseWorldPos;
//
//	m_Camera.zoom = zoomLevel;
//}

void CanvasViewControlScript::AdjustFilterToZoomLevel(float zoom, Texture2D& texture)
{
	if (zoom < 1.0f)
		SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
	else
		SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}

//void CanvasViewControlScript::MoveCanvasWithMouse()
//{
//	Vector2 delta = GetMouseDelta();
//	delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);
//	m_Camera.target = Vector2Add(m_Camera.target, delta);
//}
