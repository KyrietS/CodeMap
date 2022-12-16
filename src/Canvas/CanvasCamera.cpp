#include "pch.hpp"
#include "CanvasCamera.hpp"
#include "raymath.h"
#include <glm/vec2.hpp>


CanvasCamera::CanvasCamera()
{
	m_Camera.offset = { 0, 0 };
	m_Camera.target = { 0, 0 };
	m_Camera.rotation = 0.0f;
	m_Camera.zoom = 1.0f;
}

glm::vec2 CanvasCamera::GetScreenToWorld(glm::vec2 screenTarget)
{
	Vector2 pos = { screenTarget.x, screenTarget.y };
	Vector2 result = GetScreenToWorld2D(pos, m_Camera);
	return { result.x, result.y };
}

glm::vec2 CanvasCamera::GetWorldToScreen(glm::vec2 worldTarget)
{
	Vector2 pos = { worldTarget.x, worldTarget.y };
	Vector2 result = GetWorldToScreen2D(pos, m_Camera);
	return { result.x, result.y };
}

void CanvasCamera::CenterAtScreen(glm::vec2 screenTarget)
{
	Vector2 target = { screenTarget.x, screenTarget.y };
	Vector2 pos = GetScreenToWorld2D(target, m_Camera);
	CenterAtWorld({pos.x, pos.y});
}

void CanvasCamera::CenterAtWorld(glm::vec2 worldTarget)
{
	m_Camera.target = { worldTarget.x, worldTarget.y };
	m_Camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
}

void CanvasCamera::MoveOnScreenBy(glm::vec2 screenDelta)
{
	glm::vec2 worldDelta = screenDelta * (-1.0f / m_Camera.zoom);
	MoveInWorldBy(worldDelta);
}

void CanvasCamera::MoveInWorldBy(glm::vec2 worldDelta)
{
	glm::vec2 result = glm::vec2{ m_Camera.target.x, m_Camera.target.y } + worldDelta;
	m_Camera.target = { result.x, result.y };
}

float CanvasCamera::GetZoom()
{
	return m_Camera.zoom;
}

glm::vec2 CanvasCamera::GetCenter()
{
	Vector2 center = { GetScreenCenter().x, GetScreenCenter().y };
	Vector2 result = GetScreenToWorld2D(center,  m_Camera);
	return { result.x, result.y };
}

CameraData CanvasCamera::GetData()
{
	glm::vec2 offset = { m_Camera.offset.x, m_Camera.offset.y };
	glm::vec2 target = { m_Camera.target.x, m_Camera.target.y };
	float rotation = m_Camera.rotation;
	float zoom = m_Camera.zoom;
	return CameraData{ offset, target, rotation, zoom };
}

void CanvasCamera::SetZoom(float zoomLevel)
{
	SetZoomAt(GetScreenCenter(), zoomLevel);
}

void CanvasCamera::SetZoomAt(glm::vec2 screenPos, float zoomLevel)
{
	Vector2 pos = { screenPos.x, screenPos.y };
	Vector2 worldPos = GetScreenToWorld2D(pos, m_Camera);
	m_Camera.offset = { screenPos.x, screenPos.y };
	m_Camera.target = worldPos;

	m_Camera.zoom = zoomLevel;
}
