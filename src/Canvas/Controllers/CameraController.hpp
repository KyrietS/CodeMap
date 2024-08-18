#pragma once
#include "IController.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/EventQueue.hpp"
#include "Events/MouseEvents.hpp"

namespace Controllers
{
	class CameraController : public IController
	{
	public:
		CameraController(CanvasCamera& camera, EventQueue& eventQueue) 
			: m_Camera(camera), m_EventQueue(eventQueue) {}
		void Draw() override;
		void OnEvent(Event& event) override;
	private:
		void ZoomCamera(float zoomChange);

		// Event handlers
		bool OnMouseMoved(const Events::Input::MouseMoved&);
		void OnMouseScrolled(const Events::Input::MouseScrolled&);

		CanvasCamera& m_Camera;
		EventQueue& m_EventQueue;
	};
}