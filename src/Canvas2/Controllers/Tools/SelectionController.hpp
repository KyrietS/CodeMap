#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"

namespace Controllers
{
	class SelectionController : public IController
	{
	public:
		SelectionController(CanvasCamera& camera) : m_Camera(camera) {}
		void Draw() override;
		void OnEvent(Event& event) override;

	private:
		void DrawSelection(const glm::vec2& begin, const glm::vec2& end);
		bool OnMousePressed(const Events::Input::MousePressed&);
		bool OnMouseReleased(const Events::Input::MouseReleased&);
		bool OnMouseMoved(const Events::Input::MouseMoved&);
		void ClearSelection();

		std::optional<glm::vec2> m_SelectionStart;
		std::optional<glm::vec2> m_SelectionEnd;

		CanvasCamera& m_Camera;
	};
}