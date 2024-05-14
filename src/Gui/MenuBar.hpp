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
	void SaveCanvasToFile(const std::string& filePath);
	void LoadCanvasFromFile(const std::string& filePath);

	EventQueue& m_EventQueue;
	bool m_Demo = false; // TODO: Move to GuiContext
};
}