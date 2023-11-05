#include "pch.hpp"
#include "Gui.hpp"
#include <imgui.h>
#include "Gui/MenuBar.hpp"
#include "Gui/Overlay.hpp"
#include "Gui/PropertiesWindow.hpp"
#include "Gui/Toolbar.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/GuiEvents.hpp"
#include "Events/EventQueue.hpp"

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
}

void Gui::OnUpdate()
{
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