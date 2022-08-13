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
	//void SetCameraZoomAt(float zoom);
	void AdjustFilterToZoomLevel(float zoom, Texture2D& texture);
	//void MoveCanvasWithMouse();

	float m_ZoomBeforeAnimation = 1.0f;

	Animation m_ZoomAnimation;
	AnimationVec2 m_CameraTargetAnimation;

	//struct AerialViewState
	//{
	//	bool Started = false;
	//	float StartZoom = 0.0f;
	//	float CameraZoomBeforeAerialView = 0.0f;
	//	double CurrentTime = 0;
	//	double Duration = 0;
	//} m_AerialViewState;

	CanvasCamera& m_Camera;
};