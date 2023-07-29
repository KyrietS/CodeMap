#pragma once
#include "Canvas/Deserializer/CanvasDeserializer.hpp"
#include "entt/entt.hpp"
#include "Canvas/Canvas.hpp"
#include <tinyxml2.h>

class SvgDeserializer : public CanvasDeserializer
{
public:
	SvgDeserializer(Canvas&, entt::registry&, EventQueue&);
	void Deserialize(const std::string&) override;

private:
	void DeserializeElement(const tinyxml2::XMLElement&);
	void DeserializeText(const tinyxml2::XMLElement&);
	void DeserializeArrow(const tinyxml2::XMLElement&);
	void DeserializeImage(const tinyxml2::XMLElement&);

	Canvas& m_Canvas;
	entt::registry& m_Registry;
	EventQueue& m_EventQueue;
};
