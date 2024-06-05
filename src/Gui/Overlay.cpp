#include "pch.hpp"
#include "Overlay.hpp"
#include <imgui.h>
#include "Time.hpp"
#include "Input.hpp"

namespace Gui
{
void Overlay::OnUpdate()
{
	ShowMetaInfoOverlay();
	ShowMousePositionOverlay();
}


void Overlay::ShowMetaInfoOverlay()
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
		//int zoomLevel = (int)(Canvas::Camera().GetZoom() * 100);
		int zoomLevel = 100;
		int fps = (int)Time::GetFPS();

		ImGui::Text("zoom: %d%%", zoomLevel);
		ImGui::Separator();
		ImGui::Text("%d FPS", fps);
	}
	ImGui::End();
}


void Overlay::ShowMousePositionOverlay()
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
		//auto mousePos = Input::GetWorldMousePosition(Canvas::Camera());
		auto mousePos = glm::vec2(0.0f, 0.0f);
		ImGui::Text("(%.0f, %.0f)", mousePos.x, mousePos.y);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}
}