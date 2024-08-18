#include "pch.hpp"
#include "MenuBar.hpp"
#include <imgui.h>
#include "Events/AppEvents.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/GuiEvents.hpp"

namespace Gui
{
MenuBar::MenuBar(EventQueue& eventQueue)
	: m_EventQueue(eventQueue)
{
}

void MenuBar::OnUpdate()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				m_EventQueue.Push(Events::Canvas::LoadFromFile{});
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				m_EventQueue.Push(Events::Canvas::SaveToFile {});
			}
			if (ImGui::MenuItem("Save As.."))
			{
				m_EventQueue.Push(Events::Canvas::SaveToFile{ .SaveAs = true });
			}
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				m_EventQueue.Push(Events::Gui::ShowConfirmExitDialog {});
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z"))
			{
				m_EventQueue.Push(Events::Canvas::Undo{});
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y"))
			{
				m_EventQueue.Push(Events::Canvas::Redo{});
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
			ImGui::EndMenu();
		}
		if( ImGui::BeginMenu( "View" ) )
		{
			if( ImGui::MenuItem( "Toolbox" ) ) {} // TODO: Show/Hide Toolbox
			ImGui::EndMenu();
		}
		if( ImGui::BeginMenu( "Help" ) )
		{
			if( ImGui::MenuItem( "Show ImGui Demo", nullptr, &m_Demo ) ) {}
			ImGui::Separator();
			if (ImGui::MenuItem("About"))
			{
				m_EventQueue.Push(Events::Gui::ShowAbout{});
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar(); // ImGuiStyleVar_WindowBorderSize

	if( m_Demo ) // TODO: Move somewhere else
	{
		ImGui::ShowDemoWindow( &m_Demo );
	}
}
}