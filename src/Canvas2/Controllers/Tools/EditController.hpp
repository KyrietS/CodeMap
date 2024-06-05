#pragma once
#include "Canvas2/Controllers/IController.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/KeyEvents.hpp"
#include "Events/EventQueue.hpp"
#include "Events/CanvasEvents.hpp"

namespace Controllers
{
	class EditController : public IController
	{
	public:
		EditController(CanvasCamera& camera, CanvasElements& elements, EventQueue& eventQueue) 
			: m_Camera(camera), m_Elements(elements), m_EventQueue(eventQueue) {}
		~EditController();
		void Draw() override;
		void OnEvent(Event& event) override;

	private:
		void DrawSelection(const glm::vec2& begin, const glm::vec2& end);

		// Event handlers
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

		CanvasCamera& m_Camera;
		CanvasElements& m_Elements;
		EventQueue& m_EventQueue;
	};
}