#pragma once
#include "Gui/GuiElement.hpp"

class Event;
class EventQueue;

using ImGuiID = unsigned int;

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
	std::vector<std::unique_ptr<GuiElement>> m_GuiElements;
	EventQueue& m_EventQueue;
};
}