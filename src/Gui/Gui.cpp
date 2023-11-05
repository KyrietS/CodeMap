#include "pch.hpp"
#include "Gui.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "Gui/MenuBar.hpp"
#include "Gui/Overlay.hpp"
#include "Gui/PropertiesWindow.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/GuiEvents.hpp"
#include "Events/CanvasEvents.hpp"
#include "Events/AppEvents.hpp"
#include "Events/EventQueue.hpp"
#include "Assets/AssetLoader.hpp"
#include "Render/Renderer.hpp"
#include "Canvas/Canvas.hpp"
#include "Canvas/Entity.hpp"
#include "Time.hpp"
#include "Input.hpp"

namespace Gui
{

Gui::Gui(EventQueue& eventQueue)
	: m_EventQueue(eventQueue)
{
	Assets::Icon icon = Assets::LoadNoIconRGBA();
	m_IconPlaceholder = Renderer::LoadTextureFromBytes(icon.RGBA, icon.Width, icon.Height);
}

void Gui::OnSetup(ImGuiID viewportDockSpaceId)
{
	m_GuiElements.push_back(std::make_unique<MenuBar>(m_EventQueue));
	m_GuiElements.push_back(std::make_unique<PropertiesWindow>(m_EventQueue, viewportDockSpaceId));
	m_GuiElements.push_back(std::make_unique<Overlay>());
}

void Gui::OnUpdate()
{
	ShowToolbar();
	ShowPopups();

	for (auto& guiElement : m_GuiElements)
		guiElement->OnUpdate();
}

void Gui::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Gui::ShowPopup>(BIND_EVENT(Gui::OnShowPopupEvent));

	for (auto& guiElement : m_GuiElements)
		guiElement->OnEvent(event);
}

void Gui::OnShowPopupEvent(const Events::Gui::ShowPopup& event)
{
	LOG_DEBUG("ShowPopup");
	ImGui::OpenPopup("popup_id");
}

// TODO: Move to separate file and class
void Gui::ShowToolbar()
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
	ImVec2 overlayPos = { workPos.x + padding, workPos.y + padding * 2 };
	ImVec2 positionPivot = { 0.0f, 0.0f }; 	// Left-Top corner
	ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always, positionPivot);
	ImGui::SetNextWindowViewport(viewport->ID);

	enum class Tool
	{
		None,
		Hand,
		Select,
		Arrow,
		Highlight
	};

	static Tool selectedTool = Tool::None;

	if (ImGui::Begin("Toolbar", nullptr, window_flags))
	{
		Tool tools[] = { Tool::Hand, Tool::Select, Tool::Arrow, Tool::Highlight };

		float height = 32.0f; // Icon size
		std::uintptr_t textureId = *m_IconPlaceholder;
		auto buttonSelectedColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

		for (auto tool : tools)
		{
			ImGui::PushID((int)tool);
			if (tool == selectedTool)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, buttonSelectedColor);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent background
			}
			if (ImGui::ImageButton("tool_button", (void*)textureId, ImVec2(height, height)))
			{
				selectedTool = tool; // TODO: Send event that tool has changed
			}
			ImGui::PopStyleColor();
			ImGui::PopID();
		}
	}
	ImGui::End();
}

void Gui::ShowPopups()
{
	if (ImGui::BeginPopupContextItem("popup_id"))
	{
		ImGui::Text("Example popup");
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}


}