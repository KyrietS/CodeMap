#pragma once

#include "ScriptableEntity.hpp"
#include "Components.hpp"


struct CanvasViewControlScript : ScriptableEntity
{
	CanvasViewControlScript() : m_Camera(Canvas::Camera()) {}

	void OnUpdate() override;
	void AdjustFilterToZoomLevel(float zoom, Texture2D& texture);
	void MoveCanvasWithMouse();

	Camera2D& m_Camera;
};