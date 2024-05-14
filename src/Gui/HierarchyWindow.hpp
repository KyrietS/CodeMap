#pragma once
#include "GuiElement.hpp"
#include "Events/EventQueue.hpp"

class Entity;

namespace Gui
{
	class HierarchyWindow : public GuiElement
	{
	public:
		HierarchyWindow( EventQueue&, ImGuiID dockSpaceId );
		~HierarchyWindow() = default;
		void OnUpdate() override;

	private:
		EventQueue& m_EventQueue;
	};
}