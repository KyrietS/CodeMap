#pragma once
#include "GuiElement.hpp"
#include "Events/EventQueue.hpp"

namespace Gui
{
class MenuBar : public GuiElement
{
public:
	MenuBar(EventQueue&);
	void OnUpdate() override;

private:

	EventQueue& m_EventQueue;
	bool m_Demo = false; // TODO: Move to GuiContext
};
}