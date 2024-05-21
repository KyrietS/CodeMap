#include "pch.hpp"
#include "GuiLayer.hpp"
#include <imgui.h>


GuiLayer::GuiLayer(EventQueue& eventQueue)
	: m_Gui(eventQueue)
{
}

void GuiLayer::OnUpdate()
{
	static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None
		| ImGuiDockNodeFlags_PassthruCentralNode
		| ImGuiDockNodeFlags_NoDockingInCentralNode;
	ImGuiID viewportDockSpaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockFlags);

	if (firstRun)
	{
		m_Gui.OnSetup(viewportDockSpaceId);
		firstRun = false;
	}

	m_Gui.OnUpdate();
}

void GuiLayer::OnEvent(Event& event)
{
	m_Gui.OnEvent(event);
}
