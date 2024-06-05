#pragma once
#include "Canvas2/Deserializer/CanvasDeserializer.hpp"
#include "Canvas2/CanvasCamera.hpp"
#include "Canvas2/CanvasElements.hpp"
#include "Events/EventQueue.hpp"
#include <tinyxml2.h>

class SvgDeserializer2 : public CanvasDeserializer
{
public:
	SvgDeserializer2(CanvasCamera&, CanvasElements&, EventQueue&);
	void Deserialize(const std::string&) override;

private:
	void DeserializeElement(const tinyxml2::XMLElement&);
	void DeserializeArrow(const tinyxml2::XMLElement&);
	void DeserializeShape(const tinyxml2::XMLElement&);
	void DeserializeText(const tinyxml2::XMLElement&);
	void DeserializeImage(const tinyxml2::XMLElement&);

	CanvasCamera& m_Camera;
	CanvasElements& m_Elements;
	EventQueue& m_EventQueue;
};
