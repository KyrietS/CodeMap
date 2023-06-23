#pragma once
#include "Layer.hpp"
#include "Canvas/Serializer/CanvasSerializer.hpp"

class GuiLayer : public Layer
{
public:
    GuiLayer(std::unique_ptr<CanvasSerializer>, std::unique_ptr<CanvasDeserializer>);
    void OnUpdate() override;

private:
    void ShowMainMenuBar();
    void SaveCanvasToFile(std::string_view filename);
	void LoadCanvasFromFile(const std::string& filename);
    
    std::unique_ptr<CanvasSerializer> m_SvgSerializer;
	std::unique_ptr<CanvasDeserializer> m_SvgDeserializer;
};
