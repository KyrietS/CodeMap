#include "pch.hpp"
#include "Toolbar.hpp"
#include <imgui.h>
#include "Events/CanvasEvents.hpp"
#include "Assets/AssetLoader.hpp"
#include "Render/Renderer.hpp"

namespace Gui
{
static auto LoadIconAsTexture(const Assets::Icon& icon)
{
	return Renderer::LoadTextureFromBytes(icon.RGBA, icon.Width, icon.Height);
}

Toolbar::Toolbar(EventQueue& eventQueue)
	: m_EventQueue(eventQueue)
{
	m_ToolIcons[ToolType::None] = LoadIconAsTexture(Assets::LoadNoIconRGBA());
	m_ToolIcons[ToolType::Hand] = LoadIconAsTexture(Assets::LoadHandIconRGBA());
	m_ToolIcons[ToolType::Select] = LoadIconAsTexture(Assets::LoadCursorIconRGBA());
	m_ToolIcons[ToolType::Text] = LoadIconAsTexture(Assets::LoadTextIconRGBA());
	m_ToolIcons[ToolType::Arrow] = LoadIconAsTexture(Assets::LoadArrowIconRGBA());
	m_ToolIcons[ToolType::Highlight] = LoadIconAsTexture(Assets::LoadHighlightIconRGBA());
}

void Toolbar::OnUpdate()
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

	if (ImGui::Begin("Toolbar", nullptr, window_flags))
	{
		static const ToolType tools[] = { ToolType::Hand, ToolType::Select, ToolType::Arrow, ToolType::Highlight, ToolType::Text };
		for (auto tool : tools)
		{
			ShowToolButton(tool);
		}
	}
	ImGui::End();
}

void Toolbar::ShowToolButton(ToolType tool)
{
	float height = 32.0f; // Icon size

	std::uintptr_t textureId = *m_ToolIcons[ToolType::None];
	if (m_ToolIcons.contains(tool))
	{
		textureId = *m_ToolIcons[tool];
	}

	auto buttonSelectedColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

	ImGui::PushID((int)tool);
	if (tool == m_SelectedTool)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, buttonSelectedColor);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent background
	}
	if (ImGui::ImageButton("tool_button", (void*)textureId, ImVec2(height, height)))
	{
		m_SelectedTool = tool;
		m_EventQueue.Push(Events::Canvas::SelectTool{ tool });
	}
	ImGui::PopStyleColor();
	ImGui::PopID();
}

}