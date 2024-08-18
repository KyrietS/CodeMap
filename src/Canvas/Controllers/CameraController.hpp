#pragma once
#include "IController.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Events/EventQueue.hpp"
#include "Events/MouseEvents.hpp"

namespace Controllers
{
	class CameraController : public IController
	{
	public:
		CameraController(CanvasCamera& camera, const CanvasElements& elements, EventQueue& eventQueue) 
			: m_Camera(camera), m_Elements(elements), m_EventQueue(eventQueue) {}
		void OnEvent(Event& event) override;

	private:
		void ZoomCamera(float zoomChange);

		// Event handlers
		bool OnMouseMoved(const Events::Input::MouseMoved&);
		void OnMouseScrolled(const Events::Input::MouseScrolled&);

		CanvasCamera& m_Camera;
		const CanvasElements& m_Elements;
		EventQueue& m_EventQueue;
	};
}