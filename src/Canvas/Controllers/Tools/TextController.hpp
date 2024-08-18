#pragma once
#include "Canvas/Controllers/IController.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventQueue.hpp"

namespace Controllers
{
	class TextController : public IController
	{
	public:
		TextController(CanvasCamera& camera, CanvasElements& elements, EventQueue& eventQueue);
		void OnEvent(Event& event) override;

	private:
		bool OnMouseReleased(Events::Input::MouseReleased&);
		bool OnPasteText(Events::Canvas::PasteText&);

		void AddTextToCanvas(const std::string&);

		CanvasCamera& m_Camera;
		CanvasElements& m_Elements;
		EventQueue& m_EventQueue;
	};
}