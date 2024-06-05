#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "Render/CameraData.hpp"


class CanvasCamera
{
public:
	CanvasCamera();
	CanvasCamera(const CanvasCamera&) = delete;

	glm::vec2 GetScreenToWorld(glm::vec2 screenTarget) const;
	glm::vec2 GetWorldToScreen(glm::vec2 worldTarget);
	glm::vec2 GetCenter();
	CameraData GetData();

	void CenterAtScreen(glm::vec2 screenTarget);
	void CenterAtWorld(glm::vec2 worldTarget);

	void MoveOnScreenBy(glm::vec2 screenDelta);
	void MoveInWorldBy(glm::vec2 worldDelta);

	float GetZoom() const;
	void SetZoom(float zoomLevel);
	void SetZoomAt(glm::vec2 screenPos, float zoomLevel);

	glm::vec2 GetScreenCenter();

	glm::mat4 GetTransform() const;

private:
	glm::vec2 m_Offset;         // Camera offset (displacement from target)
	glm::vec2 m_Target;         // Camera target (rotation and zoom origin)
	float m_Rotation;           // Camera rotation in degrees. DO NOT USE!
	float m_Zoom;               // Camera zoom (scaling), should be 1.0f by default
};

