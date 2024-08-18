#include "pch.hpp"
#include "About.hpp"
#include <imgui.h>
#include "Events/GuiEvents.hpp"
#include "Events/EventDispatcher.hpp"

namespace Gui
{
void About::OnUpdate()
{
	if (m_ShouldOpenPopup)
	{
		ImGui::OpenPopup("About##about_popup");
		m_ShouldOpenPopup = false;
		LOG_DEBUG("Opened about popup");
	}

	ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;

	bool open = true;
	if (ImGui::BeginPopupModal("About##about_popup", &open, flags))
	{
		m_EventQueue.Push(EmptyEvent {}); // Opening popup in ImGui has fade animation, so keep updating the GUI

		ImGui::Text("CodeMap");
		ImGui::Separator();

		ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit;

		if (ImGui::BeginTable("about_table", 2, tableFlags))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Version:");
			ImGui::TableNextColumn();
			ImGui::Text("0.1-alpha.2");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Developed by:");
			ImGui::TableNextColumn();
			ImGui::Text("Sebastian Fojcik");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("GitHub:");
			ImGui::TableNextColumn();
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "github.com/KyrietS/CodeMap");


			ImGui::EndTable();
		}

		ImGui::EndPopup();
	}
}

void About::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Handle<Events::Gui::ShowAbout>(BIND_EVENT(About::OnShowAboutEvent));
}

bool About::OnShowAboutEvent(const Events::Gui::ShowAbout& event)
{
	m_ShouldOpenPopup = true;
	return true;
}

}