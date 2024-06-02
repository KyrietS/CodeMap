#pragma once
#include "Gui/GuiElement.hpp"

class Event;
class EventQueue;
class CanvasElements;

namespace Gui
{
class Gui
{
public:
	Gui(CanvasElements&, EventQueue&);
	Gui(const Gui&) = delete;

	void OnSetup(ImGuiID);
	void OnUpdate();
	void OnEvent(Event& event);

private:
	void SetupDockSpace(ImGuiID);

	std::vector<std::unique_ptr<GuiElement>> m_GuiElements;
	CanvasElements& m_CanvasElements;
	EventQueue& m_EventQueue;

	ImGuiID m_DockMain {};
	ImGuiID m_DockRightTop {};
	ImGuiID m_DockRightBottom {};
};
}