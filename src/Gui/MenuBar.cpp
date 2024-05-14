#include "pch.hpp"
#include "MenuBar.hpp"
#include <imgui.h>
#include "Events/AppEvents.hpp"
#include "Events/CanvasEvents.hpp"

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
				LoadCanvasFromFile("test.svg");
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				SaveCanvasToFile("canvas.xml");
			}
			if (ImGui::MenuItem("Save As..")) {}
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				m_EventQueue.Push(Events::App::Quit{});
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

void MenuBar::SaveCanvasToFile(const std::string& filename)
{
	LOG_DEBUG("Saving canvas to: {}", filename);
	m_EventQueue.Push(Events::Canvas::SaveToFile{ filename });
}

void MenuBar::LoadCanvasFromFile(const std::string& filename)
{
	LOG_DEBUG("Loading canvas from: {}", filename);
	m_EventQueue.Push(Events::Canvas::LoadFromFile{ filename });
}
}