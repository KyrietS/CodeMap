#pragma once
#include "Gui/GuiElement.hpp"

class Event;
class EventQueue;

namespace Gui
{
class Gui
{
public:
	Gui(EventQueue&);
	Gui(const Gui&) = delete;

	void OnSetup(ImGuiID);
	void OnUpdate();
	void OnEvent(Event& event);

private:
	void SetupDockSpace(ImGuiID);

	std::vector<std::unique_ptr<GuiElement>> m_GuiElements;
	EventQueue& m_EventQueue;

	ImGuiID m_DockMain {};
	ImGuiID m_DockRightTop {};
	ImGuiID m_DockRightBottom {};
};
}