#include "pch.hpp"
#include "MenuBar.hpp"
#include <imgui.h>
#include "Events/AppEvents.hpp"
#include "Events/CanvasEvents.hpp"

MenuBar::MenuBar(EventQueue& eventQueue)
	: m_EventQueue(eventQueue)
{
}

void MenuBar::OnUpdate()
{
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
		ImGui::EndMainMenuBar();
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