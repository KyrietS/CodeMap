#include "pch.hpp"
#include "Gui.hpp"
#include "Gui/MenuBar.hpp"
#include "Gui/StatusBar.hpp"
#include "Gui/Overlay.hpp"
#include "Gui/PropertiesWindow.hpp"
#include "Gui/HierarchyWindow.hpp"
#include "Gui/Toolbar.hpp"
#include "Gui/Popup.hpp"
#include <imgui_internal.h>

namespace Gui
{

Gui::Gui(CanvasElements& elements, EventQueue& eventQueue)
	: m_CanvasElements(elements), m_EventQueue(eventQueue)
{
}

void Gui::OnSetup(ImGuiID viewportDockSpaceId)
{
	SetupDockSpace(viewportDockSpaceId);
	m_GuiElements.push_back(std::make_unique<MenuBar>(m_EventQueue));
	m_GuiElements.push_back(std::make_unique<Toolbar>(m_EventQueue));
	m_GuiElements.push_back(std::make_unique<StatusBar>());
	m_GuiElements.push_back(std::make_unique<PropertiesWindow>(m_EventQueue, m_DockRightBottom));
	m_GuiElements.push_back(std::make_unique<HierarchyWindow>(m_EventQueue, m_CanvasElements, m_DockRightTop));
	m_GuiElements.push_back(std::make_unique<Overlay>());
	m_GuiElements.push_back(std::make_unique<Popup>());
}

void Gui::SetupDockSpace( ImGuiID viewportDockSpaceId )
{
	// TODO: This layout will be created on every app start.
	// After user changes the layout, the layout should be saved 
	// and loaded on app start by skipping this code.
	ImGuiID dockLeft;
	ImGuiID dockRight;
	ImGuiID dockRightTop;
	ImGuiID dockRightBottom;

	// |---------------------|----------|
	// |                     |          |
	// |                     | RightTop |
	// |                     |          |
	// |        Left         |----------|
	// |                     | Right    |
	// |                     | Bottom   |
	// |                     |          |
	// |---------------------|----------|
	ImGui::DockBuilderSplitNode( viewportDockSpaceId, ImGuiDir_Right, 0.25f, &dockRight, &dockLeft );
	ImGui::DockBuilderSplitNode( dockRight, ImGuiDir_Down, 0.75f, &dockRightBottom, &dockRightTop );

	m_DockMain = dockLeft;
	m_DockRightTop = dockRightTop;
	m_DockRightBottom = dockRightBottom;

	ImGui::DockBuilderFinish( viewportDockSpaceId );
}

void Gui::OnUpdate()
{
	for (auto& guiElement : m_GuiElements)
		guiElement->OnUpdate();
}

void Gui::OnEvent(Event& event)
{
	for (auto& guiElement : m_GuiElements)
		guiElement->OnEvent(event);
}
}