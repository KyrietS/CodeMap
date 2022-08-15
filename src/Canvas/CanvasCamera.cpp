#include "pch.hpp"
#include "CanvasCamera.hpp"
#include "raymath.h"


CanvasCamera::CanvasCamera()
{
	m_Camera.offset = { 0, 0 };
	m_Camera.target = { 0, 0 };
	m_Camera.rotation = 0.0f;
	m_Camera.zoom = 1.0f;
}

Vector2 CanvasCamera::GetScreenToWorld(Vector2 screenTarget)
{
	return GetScreenToWorld2D(screenTarget, m_Camera);
}

Vector2 CanvasCamera::GetWorldToScreen(Vector2 worldTarget)
{
	return GetWorldToScreen2D(worldTarget, m_Camera);
}

void CanvasCamera::CenterAtScreen(Vector2 screenTarget)
{
	CenterAtWorld(GetScreenToWorld2D(screenTarget, m_Camera));
}

void CanvasCamera::CenterAtWorld(Vector2 worldTarget)
{
	m_Camera.target = worldTarget;
	m_Camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
}

void CanvasCamera::MoveOnScreenBy(Vector2 screenDelta)
{
	Vector2 worldDelta = Vector2Scale(screenDelta, -1.0f / m_Camera.zoom);
	MoveInWorldBy(worldDelta);
}

void CanvasCamera::MoveInWorldBy(Vector2 worldDelta)
{
	m_Camera.target = Vector2Add(m_Camera.target, worldDelta);
}

float CanvasCamera::GetZoom()
{
	return m_Camera.zoom;
}

Vector2 CanvasCamera::GetCenter()
{
	return GetScreenToWorld2D(GetScreenCenter(), m_Camera);
}

void CanvasCamera::SetZoom(float zoomLevel)
{
	SetZoomAt(GetScreenCenter(), zoomLevel);
}

void CanvasCamera::SetZoomAt(Vector2 screenPos, float zoomLevel)
{
	Vector2 worldPos = GetScreenToWorld2D(screenPos, m_Camera);
	m_Camera.offset = screenPos;
	m_Camera.target = worldPos;

	m_Camera.zoom = zoomLevel;
}
