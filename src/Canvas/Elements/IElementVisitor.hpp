#pragma once

namespace Elements {

class ArrowElement;
class ImageElement;
class ShapeElement;
class TextElement;

class IElementVisitor
{
public:
	virtual ~IElementVisitor() = default;
	virtual void Visit(ArrowElement&) = 0;
	virtual void Visit(ImageElement&) = 0;
	virtual void Visit(ShapeElement&) = 0;
	virtual void Visit(TextElement&) = 0;
};
}
