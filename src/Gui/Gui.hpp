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

class Gui
{
public:
	Gui(EventQueue&);
	Gui(const Gui&) = delete;
	~Gui();

	void OnUpdate();
	void OnEvent(Event& event);
	void SetupDockSpace(ImGuiID viewportDockSpaceId);

private:
	void OnShowPropertiesEvent(const Events::Gui::ShowProperties&);
	void OnShowPopupEvent(const Events::Gui::ShowPopup&);

	void ShowToolbar();
	void ShowMetaInfoOverlay();
	void ShowMousePositionOverlay();

	void ShowProperties();
	void ShowPropertiesFor(Components::Transform&);
	void ShowPropertiesFor(Components::Text&);
	void ShowPropertiesFor(Components::Arrow&);
	void ShowPropertiesFor(Components::Highlight&);

	void ShowPopups();

	std::vector<std::unique_ptr<GuiElement>> m_GuiElements;

	EventQueue& m_EventQueue;
	std::unique_ptr<Entity> m_SelectedEntity;
	std::shared_ptr<unsigned int> m_IconPlaceholder;
};
