#include "pch.hpp"
#include "PropertiesWindow.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "Events/EventDispatcher.hpp"
#include "Canvas2/Elements/ArrowElement.hpp"
#include "Canvas2/Elements/ShapeElement.hpp"
#include "Canvas2/Elements/TextElement.hpp"
#include "Canvas2/Elements/ImageElement.hpp"

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
PropertiesWindow::PropertiesWindow(EventQueue& eventQueue, CanvasElements& elements, ImGuiID dockSpaceId )
	: m_EventQueue(eventQueue), m_Elements(elements)
{
	ImGui::DockBuilderDockWindow( "Properties", dockSpaceId );
	SetupDockSpace( dockSpaceId );
}

void PropertiesWindow::OnUpdate()
{
	ImGui::Begin( "Properties" );
	
	if (m_SelectedElement)
	{
		ShowProperties();
	}
	else
	{
		ImGui::Text( "No element selected." );
	}

	ImGui::End();

}

void PropertiesWindow::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Events::Gui::ShowProperties>(BIND_EVENT(PropertiesWindow::OnShowPropertiesEvent));
	dispatcher.Handle<Events::Gui::ShowProperties2>(BIND_EVENT(PropertiesWindow::OnShowProperties2));
}

// TODO: Remove this function when all properties are move to Properties window
void PropertiesWindow::SetupDockSpace(ImGuiID dockSpaceId)
{
	ImGui::DockBuilderDockWindow("Transform", dockSpaceId);
	ImGui::DockBuilderDockWindow("Arrow", dockSpaceId);
	ImGui::DockBuilderDockWindow("Text", dockSpaceId);
	ImGui::DockBuilderDockWindow("Highlight", dockSpaceId);
}

void PropertiesWindow::OnShowPropertiesEvent(const Events::Gui::ShowProperties& event)
{
	m_SelectedEntity = event.Entity ? std::make_unique<Entity>(event.Entity) : nullptr;
}

bool PropertiesWindow::OnShowProperties2(const Events::Gui::ShowProperties2& event)
{
	m_SelectedElement = event.ElementId;
	return true;
}

void PropertiesWindow::ShowProperties()
{
	if (not m_Elements.Contains(m_SelectedElement))
		return;

	auto* element = m_Elements.TryGet(m_SelectedElement);
	assert(element);

	if (auto* arrow = element->As<Elements::ArrowElement>())
	{
		ShowPropertiesFor(*arrow);
	}
	else if (auto* shape = element->As<Elements::ShapeElement>())
	{
		ShowPropertiesFor(*shape);
	}

	return;
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

void ShowPositionControl(Elements::IElement& element)
{
	glm::vec2 newPos = element.GetBoundingBox().GetPosition();
	if (ImGui::DragFloat2("Position", &newPos[ 0 ], 1.0f, 0.0f, 0.0f, "%.0f"))
	{
		glm::vec2 offset = newPos - element.GetBoundingBox().GetPosition();
		element.MoveBy(offset.x, offset.y);
	}
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
}

void PropertiesWindow::ShowPropertiesFor(Elements::ArrowElement& arrow)
{
	auto& data = arrow.GetData();

	ImGui::Text("Arrow");
	ImGui::Separator();
	ShowPositionControl(arrow);

	ImGui::ColorEdit3("Arrowhead", &data.ArrowheadColor[ 0 ], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayHex);
	ImGui::ColorEdit3("Line", &data.StrokeColor[ 0 ], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayHex);
	ImGui::DragFloat("Thickness", &data.Thickness, 0.5f, 1.0f, 256.0f, "%.1f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
}

void PropertiesWindow::ShowPropertiesFor(Elements::ShapeElement& shape)
{
	auto& data = shape.GetData();

	ImGui::Text("Shape");
	ImGui::Separator();
	ShowPositionControl(shape);

	ImGui::ColorPicker4("Color", &data.Color[ 0 ],
		ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);

	const char* items[] = { "Alpha", "Multiply" };
	int currentOption = static_cast<int>(data.BlendMode);
	ImGui::Combo("Blending", &currentOption, items, IM_ARRAYSIZE(items));
	data.BlendMode = static_cast<Render::BlendMode>(currentOption);

	ImGui::SameLine(); HelpMarker("Set the blending mode for rendering polygon.");
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