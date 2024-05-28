#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"

namespace Elements
{
	class ShapeElement;
}

namespace Controllers
{
	class ShapeController : public IController
	{
	public:
		ShapeController(CanvasCamera& camera, CanvasElements& elements);
		void Draw() override;
		void OnEvent(Event& event) override;

	private:
		void OnUpdate();
		bool OnMousePressed(Events::Input::MousePressed& event);
		bool OnMouseReleased(Events::Input::MouseReleased& event);

		void AddShapeToCanvas();

		std::unique_ptr<Elements::ShapeElement> m_Shape;

		CanvasCamera& m_Camera;
		CanvasElements& m_Elements;
	};
}