#include "pch.hpp"
#include "PropertiesWindow.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "Events/EventDispatcher.hpp"
#include "Canvas/Elements/ArrowElement.hpp"
#include "Canvas/Elements/ShapeElement.hpp"
#include "Canvas/Elements/TextElement.hpp"
#include "Canvas/Elements/ImageElement.hpp"

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

void Separator(int marginTop = 5, int marginBottom = 5)
{
	if (marginTop) ImGui::Dummy(ImVec2(0, marginTop));
	ImGui::Separator();
	if (marginBottom) ImGui::Dummy(ImVec2(0, marginBottom));
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
	
	if (m_SelectedElements.empty())
	{
		ImGui::Text( "No element selected" );
	}
	else
	{
		ShowProperties();
	}

	ImGui::End();

}

void PropertiesWindow::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Handle<Events::Gui::ShowProperties>(BIND_EVENT(PropertiesWindow::OnShowProperties2));
}

// TODO: Remove this function when all properties are move to Properties window
void PropertiesWindow::SetupDockSpace(ImGuiID dockSpaceId)
{
	ImGui::DockBuilderDockWindow("Transform", dockSpaceId);
	ImGui::DockBuilderDockWindow("Arrow", dockSpaceId);
	ImGui::DockBuilderDockWindow("Text", dockSpaceId);
	ImGui::DockBuilderDockWindow("Highlight", dockSpaceId);
}

bool PropertiesWindow::OnShowProperties2(const Events::Gui::ShowProperties& event)
{
	m_SelectedElements = event.Elements;
	return true;
}

void PropertiesWindow::ShowProperties()
{
	assert(not m_SelectedElements.empty());

	if (m_SelectedElements.size() > 1)
	{
		ShowPropertiesForMultipleElements();
	}
	else
	{
		ShowPropertiesFor(m_SelectedElements.front());
	}
}


void PropertiesWindow::ShowPropertiesFor(ElementId elementId)
{
	if (not m_Elements.Contains(elementId))
	{
		LOG_WARN("[GUI] Element with id {} not found", elementId);
		std::erase(m_SelectedElements, elementId);
		return;
	}

	auto& element = m_Elements.Get(elementId);

	if (auto* arrow = element.As<Elements::ArrowElement>())
	{
		ShowPropertiesFor(*arrow);
	}
	else if (auto* shape = element.As<Elements::ShapeElement>())
	{
		ShowPropertiesFor(*shape);
	}
	else if (auto* text = element.As<Elements::TextElement>())
	{
		ShowPropertiesFor(*text);
	}
	else if (auto* image = element.As<Elements::ImageElement>())
	{
		ShowPropertiesFor(*image);
	}
}

void PropertiesWindow::ShowPropertiesFor(Elements::ArrowElement& arrow)
{
	auto& data = arrow.GetData();

	ImGui::Text("Arrow");
	Separator(0);
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
	Separator(0);
	ShowPositionControl(shape);

	ImGui::ColorPicker4("Color", &data.Color[ 0 ],
		ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);

	const char* items[] = { "Alpha", "Multiply" };
	int currentOption = static_cast<int>(data.BlendMode);
	ImGui::Combo("Blending", &currentOption, items, IM_ARRAYSIZE(items));
	data.BlendMode = static_cast<Render::BlendMode>(currentOption);

	ImGui::SameLine(); HelpMarker("Set the blending mode for rendering polygon.");
}

void PropertiesWindow::ShowPropertiesFor(Elements::TextElement& text)
{
	auto& data = text.GetData();

	ImGui::Text("Text");
	Separator(0);
	ShowPositionControl(text);

	std::string utf8Content = data.GetTextInUtf8();
	if (ImGui::InputTextMultiline("Content", &utf8Content))
	{
		data.SetUtf8Text(utf8Content);
	}
	ImGui::DragFloat("Font size", &data.FontSize, 1.0f, 4.0f, 256.0f, "%.0f");
	if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

	ImGui::ColorEdit4("Font color", &data.FontColor[0], ImGuiColorEditFlags_NoAlpha);
}

void PropertiesWindow::ShowPropertiesFor(Elements::ImageElement& image)
{
	auto& data = image.GetData();

	ImGui::Text("Image");
	Separator(0);
	ShowPositionControl(image);

	ImGui::BeginDisabled();
	int size[] = { data.Width, data.Height };
	if (ImGui::InputInt2("Dimension", size))
	{
		data.Width = size[ 0 ];
		data.Height = size[ 1 ];
	}
	float scale = 1.0f;
	if (ImGui::DragFloat("Scale", &scale, 0.01f, 0.1f, 10.0f, "%.2f"))
	{
		// Not implemented yet
	}
	ImGui::EndDisabled();

	Separator();
	int originalWidth = data.Width;
	int originalHeight = data.Height;
	ImGui::Text("Original size: %d x %d px", originalWidth, originalHeight);
}

void PropertiesWindow::ShowPropertiesForMultipleElements()
{
	ImGui::Text("Selected %d elements", m_SelectedElements.size());
	// TODO: Show common properties for multiple elements
}
}