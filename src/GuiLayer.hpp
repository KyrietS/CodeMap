#pragma once
#include "Layer.hpp"
#include "Canvas/Serializer/CanvasSerializer.hpp"
#include "Events/EventQueue.hpp"

class GuiLayer : public Layer
{
public:
	GuiLayer(EventQueue&);
	void OnUpdate() override;

private:
	void ShowMainMenuBar();
	void SaveCanvasToFile(const std::string& filename);
	void LoadCanvasFromFile(const std::string& filename);

	EventQueue& m_EventQueue;
};
