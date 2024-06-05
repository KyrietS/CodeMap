#pragma once
#include <string>

class CanvasDeserializer
{
public:
	virtual ~CanvasDeserializer() = default;
	virtual void Deserialize(const std::string&) = 0;
};

