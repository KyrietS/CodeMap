#pragma once
#include "Gui/GuiElement.hpp"

namespace Gui
{
class Overlay : public GuiElement
{
public:
	void OnUpdate() override;

private:
	void ShowMetaInfoOverlay();
	void ShowMousePositionOverlay();
};
}