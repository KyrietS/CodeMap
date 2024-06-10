#pragma once
#include "Canvas/Deserializer/CanvasDeserializer.hpp"
#include "Canvas/CanvasCamera.hpp"
#include "Canvas/CanvasElements.hpp"
#include "Events/EventQueue.hpp"
#include <tinyxml2.h>

class SvgDeserializer : public CanvasDeserializer
{
public:
	SvgDeserializer(CanvasCamera&, CanvasElements&, EventQueue&);
	void Deserialize(const std::string&) override;

private:
	void DeserializeElement(const tinyxml2::XMLElement&);
	void DeserializeArrow(const tinyxml2::XMLElement&);
	void DeserializeShape(const tinyxml2::XMLElement&);
	void DeserializeText(const tinyxml2::XMLElement&);
	void DeserializeImage(const tinyxml2::XMLElement&);
	void LoadDefs(const tinyxml2::XMLElement&);
	void LoadMarker(const tinyxml2::XMLElement&);

	CanvasCamera& m_Camera;
	CanvasElements& m_Elements;
	EventQueue& m_EventQueue;

	std::map<std::string, glm::vec4> m_ArrowheadColors;
};
