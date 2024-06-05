#pragma once
#include "IController.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"

namespace Controllers
{
	class CameraController : public IController
	{
	public:
		CameraController(CanvasCamera& camera) : m_Camera(camera) {}
		void Draw() override;
		void OnEvent(Event& event) override;
	private:
		void ZoomCamera(float zoomChange);

		// Event handlers
		bool OnMouseMoved(const Events::Input::MouseMoved&);
		void OnMouseScrolled(const Events::Input::MouseScrolled&);

		CanvasCamera& m_Camera;
	};
}