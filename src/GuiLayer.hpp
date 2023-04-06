#pragma once
#include "Layer.hpp"
#include "Canvas/Serializer/CanvasSerializer.hpp"

class GuiLayer : public Layer
{
public:
    GuiLayer(std::unique_ptr<CanvasSerializer>);
    void OnUpdate() override;

private:
    void ShowMainMenuBar();
    void SaveCanvasToFile(std::string_view filename);
    
    std::unique_ptr<CanvasSerializer> m_SvgSerializer;
};
