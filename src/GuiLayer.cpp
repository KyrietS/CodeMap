#include <Canvas/Deserializer/CanvasDeserializer.hpp>
#include "Events/AppEvents.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/EventDispatcher.hpp"
#include "pch.hpp"
#include "GuiLayer.hpp"
#include "imgui.h"
#include "Timer.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Components.hpp"
#include "App.hpp"
#include "Input.hpp"

void ShowMetaInfoOverlay()
{
	ImGuiWindowFlags window_flags = 
		ImGuiWindowFlags_NoDecoration 
		| ImGuiWindowFlags_NoDocking 
		| ImGuiWindowFlags_AlwaysAutoResize 
		| ImGuiWindowFlags_NoSavedSettings 
		| ImGuiWindowFlags_NoFocusOnAppearing 
		| ImGuiWindowFlags_NoNav 
		| ImGuiWindowFlags_NoMove;

	const float padding = 10.0f;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 workPos = viewport->WorkPos; // Position of Top-Left corner of working area (without menubar)
	ImVec2 workSize = viewport->WorkSize;
	ImVec2 overlayPos = { padding, workPos.y + workSize.y - padding };
	ImVec2 positionPivot = { 0.0f, 1.0f }; // Left-Bottom corner
	ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always, positionPivot);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("MetaInfoOverlay", nullptr, window_flags))
	{
		int zoomLevel = (int)(Canvas::Camera().GetZoom() * 100);
		int fps = (int)Time::GetFPS();

		ImGui::Text("zoom: %d%%", zoomLevel);
		ImGui::Separator();
		ImGui::Text("%d FPS", fps);
	}
	ImGui::End();
}

void ShowMousePositionOverlay()
{
	// Show mouse position in the Right-Bottom corner
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoNav
		| ImGuiWindowFlags_NoMove;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	const float padding = 10.0f;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 workPos = viewport->WorkPos; // Position of Top-Left corner of working area (without menubar)
	ImVec2 workSize = viewport->WorkSize;
	ImVec2 overlayPos = { workPos.x + workSize.x - padding, workPos.y + workSize.y - padding };
	ImVec2 positionPivot = { 1.0f, 1.0f }; // Right-Bottom corner
	ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always, positionPivot);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("MousePositionOverlay", nullptr, window_flags))
	{
		auto mousePos = Input::GetWorldMousePosition();
		ImGui::Text("(%.0f, %.0f)", mousePos.x, mousePos.y);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void GuiLayer::SaveCanvasToFile(const std::string& filename)
{
	LOG_DEBUG("Saving canvas to: {}", filename);
	m_EventQueue.Push(Events::Canvas::SaveToFile{ filename });
}

void GuiLayer::LoadCanvasFromFile(const std::string& filename)
{
	LOG_DEBUG("Loading canvas from: {}", filename);
	m_EventQueue.Push(Events::Canvas::LoadFromFile{ filename });
}

void GuiLayer::ShowMainMenuBar()
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

void GuiLayer::ShowProperties()
{
	ImGui::Begin("Properties");
	if (m_SelectedText)
		ImGui::Text(m_SelectedText->get().Content.c_str());
	else
		ImGui::Text("No text selected");

	//static char text[1024 * 16] =
	//	"/*\n"
	//	" The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
	//	" the hexadecimal encoding of one offending instruction,\n"
	//	" more formally, the invalid operand with locked CMPXCHG8B\n"
	//	" instruction bug, is a design flaw in the majority of\n"
	//	" Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
	//	" processors (all in the P5 microarchitecture).\n"
	//	"*/\n\n"
	//	"label:\n"
	//	"\tlock cmpxchg8b eax\n";

	//static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
	//ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
	//ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
	//ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", &flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
	//ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);

	ImGui::End();
}

GuiLayer::GuiLayer(EventQueue& eventQueue)
	: m_EventQueue(eventQueue) {}

void GuiLayer::OnUpdate()
{
	ShowMainMenuBar();

	static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None 
		| ImGuiDockNodeFlags_PassthruCentralNode
		| ImGuiDockNodeFlags_NoDockingInCentralNode;
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockFlags);

	ShowProperties();
	ShowMetaInfoOverlay();
	ShowMousePositionOverlay();

	if (ImGui::BeginPopupContextItem("popup_id"))
	{
		ImGui::Text("Edit name:");
		//ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}

void GuiLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Gui::ShowPopup>(BIND_EVENT(GuiLayer::OnShowPopupEvent));
	dispatcher.Dispatch<Events::Gui::ShowProperties<Components::Text>>([&](auto& e) { m_SelectedText = e.Component; });
}

void GuiLayer::OnShowPopupEvent(const Events::Gui::ShowPopup& event)
{
	LOG_DEBUG("ShowPopup");
	ImGui::OpenPopup("popup_id");
}
