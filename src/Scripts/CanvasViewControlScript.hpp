#pragma once

#include "Canvas/ScriptableEntity.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Animation.hpp"
#include "reasings.h"


struct CanvasViewControlScript : ScriptableEntity
{
	CanvasViewControlScript() : m_Camera(Canvas::Camera()) {}

	void OnUpdate() override;
	void ZoomCamera(float zoomChange);
	void AdjustFilterToZoomLevel(float zoom, const Components::Image& texture);

	float m_ZoomBeforeAnimation = 1.0f;

	Animation m_ZoomAnimation;
	AnimationVec2 m_CameraTargetAnimation;

	CanvasCamera& m_Camera;
};