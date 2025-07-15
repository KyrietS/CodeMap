#include "pch.hpp"
#include "CanvasCamera.hpp"
#include "Window.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

CanvasCamera::CanvasCamera()
{
	m_Offset = { 0, 0 };
	m_Target = { 0, 0 };
	m_Rotation = 0.0f;
	m_Zoom = 1.0f;
}

glm::mat4 CanvasCamera::GetTransform() const
{
	glm::mat4 offset = glm::translate(glm::vec3{ m_Offset.x, m_Offset.y, 0.0f });
	glm::mat4 target = glm::translate(glm::vec3{ -m_Target.x, -m_Target.y, 0.0f });
	glm::mat4 scale = glm::scale(glm::vec3{ m_Zoom, m_Zoom, 1.0f });
	glm::mat4 rotation = glm::rotate(glm::radians(m_Rotation), glm::vec3{ 0.0f, 0.0f, 1.0f });

	return offset * ((rotation * scale) * target);
}

glm::vec2 CanvasCamera::GetScreenToWorld(glm::vec2 screenTarget) const
{
	return glm::inverse(GetTransform()) * glm::vec4{ screenTarget.x, screenTarget.y, 0.0f, 1.0f };
}

glm::vec2 CanvasCamera::GetWorldToScreen(glm::vec2 worldTarget)
{
	return GetTransform() * glm::vec4{ worldTarget.x, worldTarget.y, 0.0f, 1.0f };
}

void CanvasCamera::CenterAtScreen(glm::vec2 screenTarget)
{
	CenterAtWorld(GetScreenToWorld(screenTarget));
}

void CanvasCamera::CenterAtWorld(glm::vec2 worldTarget)
{
	m_Target = worldTarget;
	m_Offset = GetScreenCenter();
}

void CanvasCamera::MoveOnScreenBy(glm::vec2 screenDelta)
{
	glm::vec2 worldDelta = screenDelta * (-1.0f / m_Zoom);
	MoveInWorldBy(worldDelta);
}

void CanvasCamera::MoveInWorldBy(glm::vec2 worldDelta)
{
	m_Target = m_Target + worldDelta;
}

float CanvasCamera::GetZoom() const
{
	return m_Zoom;
}

glm::vec2 CanvasCamera::GetCenter()
{
	return GetScreenToWorld(GetScreenCenter());
}

CameraData CanvasCamera::GetData()
{
	return CameraData{ m_Offset, m_Target, m_Rotation, m_Zoom };
}

void CanvasCamera::SetZoom(float zoomLevel)
{
	SetZoomAt(GetScreenCenter(), zoomLevel);
}

void CanvasCamera::SetZoomAt(glm::vec2 screenPos, float zoomLevel)
{
	m_Target = GetScreenToWorld(screenPos);
	m_Offset = screenPos;

	m_Zoom = zoomLevel;
}

glm::vec2 CanvasCamera::GetScreenCenter()
{
	return { Window::GetWidth() / 2.0f, Window::GetHeight() / 2.0f };
}
