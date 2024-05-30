#pragma once
#include "Canvas/Serializer/CanvasSerializer.hpp"
#include <tinyxml2.h>
#include "ViewBox.hpp"
#include "Canvas2/CanvasElements.hpp"

namespace Elements
{
	class ArrowElement;
	class ShapeElement;
	class TextElement;
	class ImageElement;
}

class SvgSerializer2 : public CanvasSerializer
{
public:
	SvgSerializer2(const CanvasElements&);
	std::string Serialize() override;

private:
	ViewBox GetCanvasViewBox();
	void GenerateSvgRootElement(tinyxml2::XMLDocument&, ViewBox);
	void AddArrowheadMarkers(tinyxml2::XMLElement& root);
	void AddRobotoFont(tinyxml2::XMLElement& root);
	void SerializeAllElements(tinyxml2::XMLElement& root);
	void AddBorderToSvgDocument(tinyxml2::XMLElement& root, ViewBox);

	void SerializeArrow(tinyxml2::XMLElement& root, const Elements::ArrowElement&);
	void SerializeShape(tinyxml2::XMLElement& root, const Elements::ShapeElement&);
	void SerializeText(tinyxml2::XMLElement& root, const Elements::TextElement&);
	void SerializeTextContent(tinyxml2::XMLElement& textElement, const Elements::TextElement&);
	void SerializeImage(tinyxml2::XMLElement& root, const Elements::ImageElement&);

	const CanvasElements& m_Elements;
	static constexpr int PADDING_SIZE = 20;
};