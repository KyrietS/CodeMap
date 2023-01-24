#pragma once

class Event;

class Layer
{
public:
    virtual ~Layer() = default;

    virtual void OnUpdate() {}
    virtual void OnEvent(Event&) {}
};

