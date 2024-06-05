#include "pch.hpp"
#include "HierarchyWindow.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/CanvasEvents.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Canvas/Elements/ArrowElement.hpp"
#include "Canvas/Elements/ShapeElement.hpp"
#include "Canvas/Elements/TextElement.hpp"
#include "Canvas/Elements/ImageElement.hpp"
#include <imgui_internal.h>


namespace
{
	std::string GetElementName(Elements::IElement& element)
	{
		if (element.As<Elements::ArrowElement>())
			return "Arrow";
		if (element.As<Elements::ShapeElement>())
			return "Shape";
		if (element.As<Elements::TextElement>())
			return "Text";
		if (element.As<Elements::ImageElement>())
			return "Image";

		return "Unknown";
	}
}

namespace Gui
{
	HierarchyWindow::HierarchyWindow(EventQueue& eventQueue, CanvasElements& elements, ImGuiID dockSpaceId)
		: m_EventQueue( eventQueue ), m_Elements(elements)
	{
		ImGui::DockBuilderDockWindow( "Hierarchy", dockSpaceId );
	}

	void HierarchyWindow::OnUpdate()
	{
		// TODO: Implement scrolling to a specific row in the table
		// Index 0 is the top row in the table. Index 1 is the second row, and so on.
		// The scroll will position the target row at the top of the viewport.
		//constexpr int targetRowIndexFromTop = 8;
		//float scrollOffset = ImGui::GetFrameHeightWithSpacing();
		//scrollOffset = scrollOffset + ImGui::GetTextLineHeightWithSpacing() * targetRowIndexFromTop;
		//ImGui::SetNextWindowScroll(ImVec2(-1.0f, scrollOffset));

		ImGui::Begin( "Hierarchy" );
		ShowElementsHierarchy();
		ImGui::End();

		MakeSnapshotWhenCanvasChanged();
	}

	void HierarchyWindow::OnEvent(Event& event)
	{
	}

	void HierarchyWindow::ShowElementsHierarchy()
	{
		if (m_Elements.Empty())
		{
			ImGui::Text("Canvas is empty");
			return;
		}

		static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ContextMenuInBody;
		if (ImGui::BeginTable("hierarchy_table", 3, flags))
		{
			// Headers
			ImGui::TableSetupColumn("Element", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Details", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			for (auto& [id, element] : m_Elements)
			{
				ShowTableRowForElement(id, *element);
			}

			if (m_ElementToBeMoved)
			{
				m_Elements.Move(m_ElementToBeMoved->first, m_ElementToBeMoved->second);
				m_ElementToBeMoved.reset();
			}

			ImGui::EndTable();
		}
	}
	void HierarchyWindow::ShowTableRowForElement(ElementId elementId, Elements::IElement& element)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ShowTableCellForElement(elementId, element);
		ImGui::TableNextColumn();
		ShowTableCellForElementId(elementId);
		ImGui::TableNextColumn();
		ShowTableCellForElementData(element);
	}

	void HierarchyWindow::ShowTableCellForElement(ElementId elementId, Elements::IElement& element)
	{
		ImGui::PushID(elementId);
		std::string elementName = GetElementName(element);
		if (ImGui::Selectable(elementName.c_str(), &element.InEditMode, ImGuiSelectableFlags_SpanAllColumns))
		{
			if (not ImGui::GetIO().KeyCtrl)
			{
				UnselectAllElements();
			}
			// Add current element to selection and send SelectElements
			element.InEditMode = true;
		}
		ImGui::PopID();

		const float dragThreshold = 0.1f; // Needed to that click does not trigger mouse cursor change
		if (ImGui::IsItemHovered() and ImGui::IsItemActive() and ImGui::IsMouseDragging(0, dragThreshold))
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
		}

		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			const bool moveUp = ImGui::GetMouseDragDelta(0).y < 0.f;
			const bool moveDown = ImGui::GetMouseDragDelta(0).y > 0.f;

			if (moveUp)
			{
				LOG_DEBUG("Moving element up");
				m_ElementToBeMoved = std::make_pair(elementId, -1);
				ImGui::ResetMouseDragDelta();
				m_CanvasChanged = true;
			}
			if (moveDown)
			{
				LOG_DEBUG("Moving element down");
				m_ElementToBeMoved = std::make_pair(elementId, +1);
				ImGui::ResetMouseDragDelta();
				m_CanvasChanged = true;
			}

			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
		}
	}

	void HierarchyWindow::ShowTableCellForElementId(ElementId elementId)
	{
		ImGui::TextDisabled("%d", elementId);
	}

	static void ShowColorBox(const glm::vec4 color1, const glm::vec4 color2)
	{
		ImU32 imColor1 = ImGui::ColorConvertFloat4ToU32({ color1.r, color1.g, color1.b, color1.a });
		ImU32 imColor2 = ImGui::ColorConvertFloat4ToU32({ color2.r, color2.g, color2.b, color2.a });
		const ImVec2 size = { ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() };
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 markerMin = { pos.x, pos.y };
		ImVec2 markerMax = { pos.x + size.x, pos.y + size.y };
		ImRect bb(markerMin, markerMax);
		ImGui::ItemSize(bb);

		if (imColor1 == imColor2)
		{
			drawList->AddRectFilled(bb.Min, bb.Max, imColor1);
		}
		else
		{
			ImVec2 p1 = bb.Min;
			ImVec2 p2 = { bb.Max.x, bb.Min.y };
			ImVec2 p3 = bb.Max;
			ImVec2 p4 = { bb.Min.x, bb.Max.y };
			drawList->AddTriangleFilled(p1, p2, p4, imColor1);
			drawList->AddTriangleFilled(p2, p3, p4, imColor2);
		}

		// Border
		drawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), 0.0f);
	}

	static void ShowColorBox(const glm::vec4 color)
	{
		ShowColorBox(color, color);
	}

	void HierarchyWindow::ShowTableCellForElementData(Elements::IElement& element)
	{
		if (auto* arrow = element.As<Elements::ArrowElement>())
		{
			ShowColorBox(arrow->GetData().ArrowheadColor, arrow->GetData().StrokeColor);
		}
		else if (auto* shape = element.As<Elements::ShapeElement>())
		{
			ShowColorBox(shape->GetData().Color);
		}
		else if (auto* text = element.As<Elements::TextElement>())
		{
			ShowColorBox(text->GetData().FontColor);
			ImGui::SameLine();

			// replace '\n' with ' ' to avoid text wrapping
			std::string textData = text->GetData().GetTextInUtf8();
			std::replace(textData.begin(), textData.end(), '\n', ' ');
			ImGui::TextDisabled(textData.c_str());
		}
		else if (auto* image = element.As<Elements::ImageElement>())
		{
			ImGui::TextDisabled("%d x %d", image->GetData().Width, image->GetData().Height);
		}
	}

	void HierarchyWindow::MakeSnapshotWhenCanvasChanged()
	{
		bool isMouseButtonReleased = ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		if (m_CanvasChanged and isMouseButtonReleased)
		{
			LOG_INFO("[GUI] Hierarchy Window: canvas changed. Snapshot taken.");
			m_EventQueue.Push(Events::Canvas::MakeSnapshot {});
			m_CanvasChanged = false;
		}
	}
	void HierarchyWindow::UnselectAllElements()
	{
		for (auto& [id, element] : m_Elements)
		{
			element->InEditMode = false;
		}
	}
}