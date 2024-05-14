#include "pch.hpp"
#include "Toolbar.hpp"
#include <imgui.h>
#include "Events/CanvasEvents.hpp"
#include "Assets/AssetLoader.hpp"
#include "Render/Renderer.hpp"
#include <imgui_internal.h>

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
	ShowToolbar();
	ShowToolbox();
}

void Toolbar::ShowToolbar()
{
	ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0.f, 7.f ) );
	ImGuiWindowFlags secondaryMenuFlags =
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

	if( ImGui::BeginViewportSideBar( "Toolbar", ImGui::GetMainViewport(), ImGuiDir_Up, ImGui::GetFrameHeight(), secondaryMenuFlags ) )
	{
		if( ImGui::BeginMenuBar() )
		{
			ImGui::PopStyleVar(); // ImGuiStyleVar_FramePadding

			ImVec2 menuBarSize = ImGui::GetWindowSize();

			ImVec2 buttonPadding = { 4.f, 4.f };
			ImVec2 buttonSize = { menuBarSize.y - buttonPadding.x * 2, menuBarSize.y - buttonPadding.y * 2 };
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, buttonPadding );
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 1.f, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) ); // Transparent background
			{
				ImGui::ImageButton( "tool_btn", (void*)(std::uintptr_t)(*m_ToolIcons[ ToolType::Hand ]), buttonSize );
				ImGui::ImageButton( "tool_btn", (void*)(std::uintptr_t)(*m_ToolIcons[ ToolType::Select ]), buttonSize );
				ImGui::ImageButton( "tool_btn", (void*)(std::uintptr_t)(*m_ToolIcons[ ToolType::Arrow ]), buttonSize );
				ImGui::ImageButton( "tool_btn", (void*)(std::uintptr_t)(*m_ToolIcons[ ToolType::Highlight ]), buttonSize );
				ImGui::ImageButton( "tool_btn", (void*)(std::uintptr_t)(*m_ToolIcons[ ToolType::Text ]), buttonSize );
			}
			ImGui::PopStyleVar( 2 );
			ImGui::PopStyleColor();
			{
				ImGui::Spacing();
				ImGui::Separator();
			}
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0.f, 7.f ) );
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 1.f, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
			{
				ImGui::Button( "A", { menuBarSize.y, 0 } );
				ImGui::Button( "B", { menuBarSize.y, 0 } );
				ImGui::Button( "C", { menuBarSize.y, 0 } );
				ImGui::Button( "D", { menuBarSize.y, 0 } );
				ImGui::Button( "E", { menuBarSize.y, 0 } );
			}
			ImGui::PopStyleVar( 2 );
			ImGui::PopStyleColor();

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
}

void Toolbar::ShowToolbox()
{
	ImGuiWindowFlags toolboxWindowFlags =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav;

	if( frameNumber <= 1 )
	{
		const float padding = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 workPos = viewport->WorkPos; // Position of Top-Left corner of working area (without menubar)
		ImVec2 workSize = viewport->WorkSize;
		ImVec2 overlayPos = { workPos.x + padding, workPos.y + padding * 2 };
		ImVec2 positionPivot = { 0.0f, 0.0f }; 	// Left-Top corner
		ImGui::SetNextWindowPos( overlayPos, ImGuiCond_Always, positionPivot );
		ImGui::SetNextWindowViewport( viewport->ID );
		frameNumber++; // TODO: Move to GuiContext
	}

	if( toolboxVisible && ImGui::Begin( "Tools", &toolboxVisible, toolboxWindowFlags ) )
	{
		static const ToolType tools[] = { ToolType::Hand, ToolType::Select, ToolType::Arrow, ToolType::Highlight, ToolType::Text };
		for( auto tool : tools )
		{
			ShowToolboxButton( tool );
		}
		ImGui::End();
	}
}

void Toolbar::ShowToolboxButton(ToolType tool)
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