#pragma once

class Event;

// TODO: Move to "Layer" directory
class Layer
{
public:
    virtual ~Layer() = default;

    virtual void OnUpdate() {}
    virtual void OnEvent(Event&) {}
};

