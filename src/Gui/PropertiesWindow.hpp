#pragma once
#include "GuiElement.hpp"
#include "Events/EventQueue.hpp"
#include "Events/GuiEvents.hpp"
#include "Canvas/CanvasElements.hpp"

using TextureId = unsigned int;

namespace Elements
{
	class ArrowElement;
	class ShapeElement;
	class TextElement;
	class ImageElement;
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
	bool OnShowProperties2(const Events::Gui::ShowProperties&);

	void ShowProperties();

	void ShowPropertiesFor(ElementId);
	void ShowPropertiesFor(Elements::ArrowElement&);
	void ShowPropertiesFor(Elements::ShapeElement&);
	void ShowPropertiesFor(Elements::TextElement&);
	void ShowPropertiesFor(Elements::ImageElement&);
	void ShowPropertiesForMultipleElements();

	void ShowSetDefaultButton(Elements::IElement&);
	bool SetAsDefaultButton(bool selected);

	EventQueue& m_EventQueue;
	CanvasElements& m_Elements;
	std::vector<ElementId> m_SelectedElements;
	std::shared_ptr<TextureId> m_SetDefaultButtonIconTexture;
};
}