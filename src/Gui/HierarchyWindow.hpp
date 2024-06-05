#pragma once
#include "GuiElement.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Events/CanvasEvents.hpp"

class Entity;
class EventQueue;

namespace Gui
{
	class HierarchyWindow : public GuiElement
	{
	public:
		HierarchyWindow(EventQueue&, CanvasElements&, ImGuiID dockSpaceId);
		~HierarchyWindow() = default;
		void OnUpdate() override;
		void OnEvent(Event&) override;

	private:
		void ShowElementsHierarchy();
		void ShowTableRowForElement(ElementId, Elements::IElement&);
		void ShowTableCellForElement(ElementId, Elements::IElement&);
		void ShowTableCellForElementId(ElementId);
		void ShowTableCellForElementData(Elements::IElement&);
		void MakeSnapshotWhenCanvasChanged();

		EventQueue& m_EventQueue;
		CanvasElements& m_Elements;
		bool m_CanvasChanged = false;
		std::optional<std::pair<ElementId, int>> m_ElementToBeMoved;
	};
}