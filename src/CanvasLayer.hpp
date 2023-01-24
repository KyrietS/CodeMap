#pragma once
#include "Layer.hpp"

class Canvas;

class CanvasLayer : public Layer
{
public:
    CanvasLayer(Canvas&);

    void OnUpdate() override;
    void OnEvent(Event&) override;

private:

    Canvas& m_Canvas;
};

