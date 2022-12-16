#pragma once

// TODO Hide raylib in cpp file
#include "raylib.h"
#include <glm/vec2.hpp>
#include "Render/CameraData.hpp"

class CanvasCamera
{
public:
	CanvasCamera();
	CanvasCamera(const CanvasCamera&) = delete;

	glm::vec2 GetScreenToWorld(glm::vec2 screenTarget);
	glm::vec2 GetWorldToScreen(glm::vec2 worldTarget);
	glm::vec2 GetCenter();
	CameraData GetData();

	void CenterAtScreen(glm::vec2 screenTarget);
	void CenterAtWorld(glm::vec2 worldTarget);

	void MoveOnScreenBy(glm::vec2 screenDelta);
	void MoveInWorldBy(glm::vec2 worldDelta);

	float GetZoom();
	void SetZoom(float zoomLevel);
	void SetZoomAt(glm::vec2 screenPos, float zoomLevel);

	static glm::vec2 GetScreenCenter()
	{
		return { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	}

private:
	operator Camera2D& () 
	{ 
		return m_Camera; 
	}

private:
	Camera2D m_Camera;

	friend class Canvas;
};

