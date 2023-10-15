#pragma once
#include "Layer.hpp"
#include "Events/Event.hpp"

class DearImGuiLayer : public Layer
{
public:
    DearImGuiLayer();
    ~DearImGuiLayer();
    void OnEvent(Event&) override;

    void Begin();
    void End();

private:
    const std::vector<uint8_t> m_FontData;
};

