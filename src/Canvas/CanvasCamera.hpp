#pragma once

#include "raylib.h"


class CanvasCamera
{
public:
	CanvasCamera();
	CanvasCamera(const CanvasCamera&) = delete;

	Vector2 GetScreenToWorld(Vector2 screenTarget);
	Vector2 GetWorldToScreen(Vector2 worldTarget);
	Vector2 GetCenter();

	void CenterAtScreen(Vector2 screenTarget);
	void CenterAtWorld(Vector2 worldTarget);

	void MoveOnScreenBy(Vector2 screenDelta);
	void MoveInWorldBy(Vector2 worldDelta);

	float GetZoom();
	void SetZoom(float zoomLevel);
	void SetZoomAt(Vector2 screenPos, float zoomLevel);

	static Vector2 GetScreenCenter()
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

