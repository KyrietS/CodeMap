#pragma once
#include "Layer.hpp"
#include "Gui/Gui.hpp"

class EventQueue;
class CanvasElements;

namespace Components
{
	struct Text;
}

class GuiLayer : public Layer
{
public:
	GuiLayer(CanvasElements&, EventQueue&);
	void OnUpdate() override;
	void OnEvent(Event& event) override;

private:

	Gui::Gui m_Gui;
	bool firstRun = true;
};
