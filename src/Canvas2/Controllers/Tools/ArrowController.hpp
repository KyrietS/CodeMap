#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Events/EventQueue.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"

namespace Elements
{
	class ArrowElement;
}

namespace Controllers
{
	class ArrowController : public IController
	{
	public:
		ArrowController(CanvasCamera& camera, CanvasElements& elements, EventQueue& eventQueue);
		void Draw() override;
		void OnEvent(Event& event) override;

	private:
		void OnUpdate();
		bool OnMousePressed(Events::Input::MousePressed& event);
		bool OnMouseReleased(Events::Input::MouseReleased& event);
		bool OnKeyPressed(Events::Input::KeyPressed& event);
		
		void AddArrowToCanvas();

		std::unique_ptr<Elements::ArrowElement> m_Arrow;

		CanvasCamera& m_Camera;
		CanvasElements& m_Elements;
		EventQueue& m_EventQueue;
	};
}