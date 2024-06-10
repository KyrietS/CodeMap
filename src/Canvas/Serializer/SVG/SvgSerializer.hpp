#pragma once
#include "Canvas/Serializer/CanvasSerializer.hpp"
#include <tinyxml2.h>
#include "ViewBox.hpp"
#include "Canvas/CanvasElements.hpp"

namespace Elements
{
	class ArrowElement;
	class ShapeElement;
	class TextElement;
	class ImageElement;
}

class SvgSerializer : public CanvasSerializer
{
public:
	SvgSerializer(const CanvasElements&);
	std::string Serialize() override;

private:
	ViewBox GetCanvasViewBox();
	void GenerateSvgRootElement(tinyxml2::XMLDocument&, ViewBox);
	void AddRobotoFont(tinyxml2::XMLElement& root);
	void SerializeAllElements(tinyxml2::XMLElement& root);
	void AddBorderToSvgDocument(tinyxml2::XMLElement& root, ViewBox);

	void SerializeArrow(tinyxml2::XMLElement& root, const Elements::ArrowElement&);
	std::string AddArrowheadMarker(tinyxml2::XMLElement& root, glm::vec4 color);
	void SerializeShape(tinyxml2::XMLElement& root, const Elements::ShapeElement&);
	void SerializeText(tinyxml2::XMLElement& root, const Elements::TextElement&);
	void SerializeTextContent(tinyxml2::XMLElement& textElement, const Elements::TextElement&);
	void SerializeImage(tinyxml2::XMLElement& root, const Elements::ImageElement&);

	const CanvasElements& m_Elements;
	static constexpr int PADDING_SIZE = 20;
};