#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"

namespace Controllers
{
	class HandController : public IController
	{
	public:
		HandController(CanvasCamera& camera) : m_Camera(camera) {}
		void OnEvent(Event& event) override;

	private:
		bool OnMousePressed(const Events::Input::MousePressed&);
		bool OnMouseReleased(const Events::Input::MouseReleased&);
		bool OnMouseMoved(const Events::Input::MouseMoved&);

		bool m_IsCameraDragging = false;

		CanvasCamera& m_Camera;
	};
}