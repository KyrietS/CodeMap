#pragma once
#include "Gui/GuiElement.hpp"

namespace Events::Gui
{
struct ShowProperties;
struct ShowPopup;
}

namespace Components
{
struct Transform;
struct Text;
struct Arrow;
struct Highlight;
}

class Event;
class Entity;
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
	void OnShowPopupEvent(const Events::Gui::ShowPopup&);
	void ShowPopups();

	std::vector<std::unique_ptr<GuiElement>> m_GuiElements;
	EventQueue& m_EventQueue;
};
}