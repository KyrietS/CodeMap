#pragma once

#include "GuiElement.hpp"
#include "Events/GuiEvents.hpp"
#include "Events/EventQueue.hpp"

namespace Gui
{
class About : public GuiElement
{
public:
	About(EventQueue& eventQueue) : m_EventQueue(eventQueue) {}
	void OnUpdate() override;
	void OnEvent(Event&) override;

private:
	bool OnShowAboutEvent(const Events::Gui::ShowAbout&);

	bool m_ShouldOpenPopup = false;

	EventQueue& m_EventQueue;
};
}