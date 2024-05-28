#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"

namespace Controllers
{
	class SelectionController : public IController
	{
	public:
		SelectionController(CanvasCamera& camera, CanvasElements& elements) 
			: m_Camera(camera), m_Elements(elements) {}
		~SelectionController();
		void Draw() override;
		void OnEvent(Event& event) override;

	private:
		void DrawSelection(const glm::vec2& begin, const glm::vec2& end);
		void OnUpdate();
		bool OnMousePressed(const Events::Input::MousePressed&);
		bool OnMouseReleased(const Events::Input::MouseReleased&);
		bool OnKeyPressed(const Events::Input::KeyPressed&);
		void PassEventToSelectedElements(Event&);
		bool SelectHoveredElement();
		void HandleMouseHoveredOverElement();
		void MoveSelectedElementsBy(glm::vec2);
		void UnselectAllElements();
		void RemoveSelectedElements();
		void HideSelectionRectangle();

		bool m_MoveByDrag = false;
		glm::vec2 m_LastMouseWorldPosition {};

		std::optional<glm::vec2> m_SelectionStart;
		std::optional<glm::vec2> m_SelectionEnd;
		std::optional<ElementId> m_HoveredElement;
		std::set<ElementId> m_SelectedElements;

		CanvasCamera& m_Camera;
		CanvasElements& m_Elements;
	};
}