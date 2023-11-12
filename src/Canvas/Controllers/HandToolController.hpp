#pragma once
#include "IController.hpp"
#include "Canvas/CanvasCamera.hpp"

class HandToolController : public IController
{
public:
	HandToolController(CanvasCamera& canvasCamera) : m_Camera(canvasCamera) {}
	void OnUpdate() override;

private:
	CanvasCamera& m_Camera;
};

