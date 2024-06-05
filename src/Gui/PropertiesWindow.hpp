#pragma once
#include "GuiElement.hpp"
#include "Events/EventQueue.hpp"
#include "Events/GuiEvents.hpp"
#include "Canvas2/CanvasElements.hpp"

class Entity;

namespace Elements
{
	class ArrowElement;
	class ShapeElement;
	class TextElement;
}

namespace Gui
{
class PropertiesWindow : public GuiElement
{
public:
	PropertiesWindow(EventQueue&, CanvasElements&, ImGuiID);
	~PropertiesWindow() = default;
	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void SetupDockSpace(ImGuiID viewportDockSpaceId);
	
	void OnShowPropertiesEvent(const Events::Gui::ShowProperties&);
	bool OnShowProperties2(const Events::Gui::ShowProperties2&);

	void ShowProperties();

	void ShowPropertiesFor(Elements::ArrowElement&);
	void ShowPropertiesFor(Elements::ShapeElement&);
	void ShowPropertiesFor(Elements::TextElement&);

	void ShowPropertiesFor(Components::Transform&);
	void ShowPropertiesFor(Components::Text&);
	void ShowPropertiesFor(Components::Arrow&);
	void ShowPropertiesFor(Components::Highlight&);

	EventQueue& m_EventQueue;
	CanvasElements& m_Elements;
	ElementId m_SelectedElement = 0;
	std::unique_ptr<Entity> m_SelectedEntity;
};
}