#pragma once
#include "GuiElement.hpp"
#include "Events/EventQueue.hpp"
#include "Events/GuiEvents.hpp"

class Entity;

using ImGuiID = unsigned int;

namespace Gui
{
class PropertiesWindow : public GuiElement
{
public:
	PropertiesWindow(EventQueue&, ImGuiID);
	~PropertiesWindow() = default;
	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void SetupDockSpace(ImGuiID viewportDockSpaceId);
	
	void OnShowPropertiesEvent(const Events::Gui::ShowProperties&);

	void ShowProperties();
	void ShowPropertiesFor(Components::Transform&);
	void ShowPropertiesFor(Components::Text&);
	void ShowPropertiesFor(Components::Arrow&);
	void ShowPropertiesFor(Components::Highlight&);

	EventQueue& m_EventQueue;
	std::unique_ptr<Entity> m_SelectedEntity;
};
}