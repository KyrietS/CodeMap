#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"

namespace Controllers
{
	class TextController : public IController
	{
	public:
		TextController(CanvasCamera& camera, CanvasElements& elements);
		void OnEvent(Event& event) override;

	private:
		bool OnMouseReleased(Events::Input::MouseReleased& event);

		void AddTextToCanvas();

		CanvasCamera& m_Camera;
		CanvasElements& m_Elements;
	};
}