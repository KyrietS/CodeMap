#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/EventQueue.hpp"

namespace Controllers
{
	class TextController : public IController
	{
	public:
		TextController(CanvasCamera& camera, CanvasElements& elements, EventQueue& eventQueue);
		void OnEvent(Event& event) override;

	private:
		bool OnMouseReleased(Events::Input::MouseReleased& event);

		void AddTextToCanvas();

		CanvasCamera& m_Camera;
		CanvasElements& m_Elements;
		EventQueue& m_EventQueue;
	};
}