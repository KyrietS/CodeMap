#include "pch.hpp"
#include "Gui.hpp"
#include "Gui/MenuBar.hpp"
#include "Gui/Overlay.hpp"
#include "Gui/PropertiesWindow.hpp"
#include "Gui/Toolbar.hpp"
#include "Gui/Popup.hpp"

namespace Gui
{

Gui::Gui(EventQueue& eventQueue)
	: m_EventQueue(eventQueue)
{
}

void Gui::OnSetup(ImGuiID viewportDockSpaceId)
{
	m_GuiElements.push_back(std::make_unique<MenuBar>(m_EventQueue));
	m_GuiElements.push_back(std::make_unique<Toolbar>());
	m_GuiElements.push_back(std::make_unique<PropertiesWindow>(m_EventQueue, viewportDockSpaceId));
	m_GuiElements.push_back(std::make_unique<Overlay>());
	m_GuiElements.push_back(std::make_unique<Popup>());
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