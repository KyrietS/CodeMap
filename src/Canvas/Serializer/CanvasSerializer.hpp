#pragma once

class CanvasSerializer
{
public:
    virtual ~CanvasSerializer() = default;
	virtual std::string Serialize() = 0;
};

