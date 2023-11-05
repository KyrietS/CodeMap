#pragma once

#include "GuiElement.hpp"
#include "Events/GuiEvents.hpp"

namespace Gui
{
class Popup : public GuiElement
{
public:
	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	void OnShowPopupEvent(const Events::Gui::ShowPopup& event);
};
}