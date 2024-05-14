#include "pch.hpp"
#include "HierarchyWindow.hpp"
#include <imgui_internal.h>

namespace Gui
{
	HierarchyWindow::HierarchyWindow( EventQueue& eventQueue, ImGuiID dockSpaceId )
		: m_EventQueue( eventQueue )
	{
		ImGui::DockBuilderDockWindow( "Hierarchy", dockSpaceId );
	}

	void HierarchyWindow::OnUpdate()
	{
		ImGui::Begin( "Hierarchy" );
		// TODO
		ImGui::End();
	}
}