#pragma once
#include "IController.hpp"
#include <Canvas/CanvasCamera.hpp>
#include "Events/MouseEvent.hpp"
#include "Canvas/Animation.hpp"


class CameraController : public IController
{
public:
	CameraController(CanvasCamera& camera) : m_Camera(camera) {}
	void OnUpdate() override;

private:
	void ZoomCamera(float zoomChange);

	float m_ZoomBeforeAnimation = 1.0f;

	Animation m_ZoomAnimation;
	AnimationVec2 m_CameraTargetAnimation;

	CanvasCamera& m_Camera;
};

