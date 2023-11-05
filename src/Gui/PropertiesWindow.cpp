#include "pch.hpp"
#include "PropertiesWindow.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "Events/EventDispatcher.hpp"

namespace
{
void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
}

namespace Gui
{
PropertiesWindow::PropertiesWindow(EventQueue& eventQueue, ImGuiID viewportDockSpaceId)
	: m_EventQueue(eventQueue)
{
	SetupDockSpace(viewportDockSpaceId);
}

void PropertiesWindow::OnUpdate()
{
	ShowProperties();
}

void PropertiesWindow::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Gui::ShowProperties>(BIND_EVENT(PropertiesWindow::OnShowPropertiesEvent));
}

void PropertiesWindow::SetupDockSpace(ImGuiID viewportDockSpaceId)
{
	ImGuiID dockRight = ImGui::DockBuilderSplitNode(viewportDockSpaceId, ImGuiDir_Right, 0.3f, nullptr, &viewportDockSpaceId);
	ImGui::DockBuilderDockWindow("Transform", dockRight);
	ImGui::DockBuilderDockWindow("Arrow", dockRight);
	ImGui::DockBuilderDockWindow("Text", dockRight);
	ImGui::DockBuilderDockWindow("Highlight", dockRight);
}

void PropertiesWindow::OnShowPropertiesEvent(const Events::Gui::ShowProperties& event)
{
	m_SelectedEntity = event.Entity ? std::make_unique<Entity>(event.Entity) : nullptr;
}

void PropertiesWindow::ShowProperties()
{
	if (!m_SelectedEntity || !m_SelectedEntity->IsValid())
		return;

	if (m_SelectedEntity->HasComponent<Components::Transform>())
	{
		ImGui::Begin("Transform");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Transform>());
		ImGui::End();
	}
	if (m_SelectedEntity->HasComponent<Components::Text>())
	{
		ImGui::Begin("Text");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Text>());
		ImGui::End();
	}
	if (m_SelectedEntity->HasComponent<Components::Arrow>())
	{
		ImGui::Begin("Arrow");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Arrow>());
		ImGui::End();
	}
	if (m_SelectedEntity->HasComponent<Components::Highlight>())
	{
		ImGui::Begin("Highlight");
		ShowPropertiesFor(m_SelectedEntity->GetComponent<Components::Highlight>());
		ImGui::End();
	}
}

void PropertiesWindow::ShowPropertiesFor(Components::Transform& transform)
{
	ImGui::Text("Transform");
	ImGui::Separator();
	ImGui::DragFloat2("Position", &transform.Translation[0], 1.0f, 0.0f, 0.0f, "%.0f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
	ImGui::InputInt("Index", &transform.Index, 1, 10);
}

void PropertiesWindow::ShowPropertiesFor(Components::Text& text)
{
	ImGui::Text("Text");
	ImGui::Separator();
	std::string utf8Content = text.GetContentInUtf8();
	ImGui::InputTextMultiline("Content", &utf8Content);
	text.SetUtf8Content(utf8Content);
	ImGui::DragFloat("Font size", &text.FontSize, 1.0f, 4.0f, 256.0f, "%.0f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

	ImGui::BeginDisabled();
	ImGui::ColorEdit4("Font color", &text.FontColor[0], ImGuiColorEditFlags_NoAlpha);
	ImGui::EndDisabled();
}

void PropertiesWindow::ShowPropertiesFor(Components::Arrow& arrow)
{
	ImGui::Text("Arrow");
	ImGui::Separator();
	ImGui::ColorPicker3("Arrow color", &arrow.StrokeColor[0],
		ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);
	ImGui::ColorEdit3("Arrowhead", &arrow.ArrowheadColor[0], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayHex);
	ImGui::DragFloat("Thickness", &arrow.Thickness, 0.5f, 0.1f, 256.0f, "%.1f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
}

void PropertiesWindow::ShowPropertiesFor(Components::Highlight& highlight)
{
	ImGui::Text("Highlight");
	ImGui::Separator();
	ImGui::ColorPicker4("Highlight color", &highlight.Color[0],
		ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);

	const char* items[] = { "Alpha", "Multiply" };
	int currentOption = static_cast<int>(highlight.BlendMode);
	ImGui::Combo("Blending", &currentOption, items, IM_ARRAYSIZE(items));
	highlight.BlendMode = static_cast<Render::BlendMode>(currentOption);

	ImGui::SameLine(); HelpMarker("Set the blending mode for Highligh object.");
}

}