#pragma once
#include "Layer.hpp"
#include "Events/EventQueue.hpp"
#include "Gui/Gui.hpp"

namespace Components
{
	struct Text;
}

class GuiLayer : public Layer
{
public:
	GuiLayer(EventQueue&);
	void OnUpdate() override;
	void OnEvent(Event& event) override;

private:

	Gui m_Gui;
	bool firstRun = true;
};
