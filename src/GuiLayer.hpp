#pragma once
#include "Layer.hpp"
#include "Canvas/Serializer/CanvasSerializer.hpp"
#include <tinyevents/tinyevents.hpp>

class GuiLayer : public Layer
{
public:
    GuiLayer(tinyevents::Dispatcher&);
    void OnUpdate() override;

private:
    void ShowMainMenuBar();
    void SaveCanvasToFile(const std::string& filename);
	void LoadCanvasFromFile(const std::string& filename);

    tinyevents::Dispatcher& m_Dispatcher;
};
