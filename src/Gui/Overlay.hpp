#pragma once
#include "Gui/GuiElement.hpp"
#include "Events/GuiEvents.hpp"

namespace Gui
{
class Overlay : public GuiElement
{
public:
	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	float zoomLevel = 1.0f;

	bool OnZoomChanged(const Events::Gui::ZoomChanged&);

	void ShowMetaInfoOverlay();
	void ShowMousePositionOverlay();
};
}