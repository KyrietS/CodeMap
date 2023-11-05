#pragma once
#include "GuiElement.hpp"
#include "Events/EventQueue.hpp"

class MenuBar : public GuiElement
{
public:
	MenuBar(EventQueue&);
	void OnUpdate() override;

private:
	void SaveCanvasToFile(const std::string& filePath);
	void LoadCanvasFromFile(const std::string& filePath);

	EventQueue& m_EventQueue;
};