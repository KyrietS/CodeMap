#include "pch.hpp"
#include "SvgSerializer2.hpp"
#include "Render/VColor.hpp"
#include "Canvas2/Elements/ArrowElement.hpp"
#include "Canvas2/Elements/ShapeElement.hpp"
#include "Canvas2/Elements/TextElement.hpp"
#include "Canvas2/Elements/ImageElement.hpp"
#include "Utils/Strings.hpp"
#include "Render/Renderer.hpp"
#include <ranges>
#include <stb_image_write.h>
#include <raylib.h> // EncodeDataBase64

namespace
{
	std::string GetColorString(glm::vec4 color)
	{
		std::stringstream ss;
		ss << "rgb(" << (int)(color.r * 255) 
			<< " " << (int)(color.g * 255) 
			<< " " << (int)(color.b * 255) 
			<< " / " << color.a << ")";
		return ss.str();
	}
}

SvgSerializer2::SvgSerializer2(const CanvasElements& canvasElements)
	: m_Elements(canvasElements) {}

std::string SvgSerializer2::Serialize()
{
	auto viewBox = GetCanvasViewBox();

	tinyxml2::XMLDocument doc;
	GenerateSvgRootElement(doc, viewBox);
	auto& root = *doc.RootElement();

	AddArrowheadMarkers(root);
	AddRobotoFont(root);
	SerializeAllElements(root);
	AddBorderToSvgDocument(root, viewBox);

	doc.SaveFile("canvas.svg");

	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}

ViewBox SvgSerializer2::GetCanvasViewBox()
{
	ViewBoxBuilder viewBoxBuilder {};
	for (const auto& [id, element] : m_Elements)
	{
		auto box = element->GetBoundingBox();
		glm::vec2 minPoint = { box.x, box.y };
		glm::vec2 maxPoint = { box.x + box.width, box.y + box.height };
		viewBoxBuilder.AddPoint(minPoint);
		viewBoxBuilder.AddPoint(maxPoint);
	}
	viewBoxBuilder.AddPadding(PADDING_SIZE);

	return viewBoxBuilder.Build();
}

void SvgSerializer2::GenerateSvgRootElement(tinyxml2::XMLDocument& doc, ViewBox viewBox)
{
	auto root = doc.NewElement("svg");
	root->SetAttribute("version", "1.1");
	root->SetAttribute("width", viewBox.Width);
	root->SetAttribute("height", viewBox.Height);
	auto viewBoxAsString = fmt::format("{} {} {} {}", viewBox.X, viewBox.Y, viewBox.Width, viewBox.Height);
	root->SetAttribute("viewBox", viewBoxAsString.c_str());
	root->SetAttribute("xmlns", "http://www.w3.org/2000/svg");

	doc.InsertFirstChild(root);
}

void SvgSerializer2::AddArrowheadMarkers(tinyxml2::XMLElement& root)
{
	// FIXME: These values are hardcoded for now and based on the values used in Canvas during rendering.
	//        We should have different types of arrowheads with their own sizes and colors.
	auto defs = root.InsertNewChildElement("defs");
	auto marker = defs->InsertNewChildElement("marker");
	marker->SetAttribute("id", "arrowhead"); // unique id
	marker->SetAttribute("viewBox", "0 0 30 20"); // arrowhead dimensions
	marker->SetAttribute("refX", 30); // x coordinate of the arrowhead's tip
	marker->SetAttribute("refY", 10); // y coordinate of the arrowhead's tip
	marker->SetAttribute("markerUnits", "strokeWidth"); // arrowhead size will be 'markerWidth/Height' times the width of the line
	marker->SetAttribute("markerWidth", 6);
	marker->SetAttribute("markerHeight", 4);
	marker->SetAttribute("orient", "auto");
	auto path = marker->InsertNewChildElement("path");
	path->SetAttribute("d", "M 0 0 L 30 10 L 0 20 z");
	path->SetAttribute("fill", GetColorString(VColor::Orange).c_str());
}

void SvgSerializer2::AddRobotoFont(tinyxml2::XMLElement& root)
{
	auto style = root.InsertNewChildElement("style");
	style->SetAttribute("type", "text/css");
	style->SetText("@import url('https://fonts.googleapis.com/css2?family=Roboto');");
}

void SvgSerializer2::AddBorderToSvgDocument(tinyxml2::XMLElement& root, ViewBox viewBox)
{
	auto border = root.InsertNewChildElement("rect");
	border->SetAttribute("x", viewBox.X);
	border->SetAttribute("y", viewBox.Y);
	border->SetAttribute("width", viewBox.Width);
	border->SetAttribute("height", viewBox.Height);
	border->SetAttribute("fill", "none");
	border->SetAttribute("stroke", "red");
	border->SetAttribute("stroke-width", 1);
}

void SvgSerializer2::SerializeAllElements(tinyxml2::XMLElement& root)
{
	for (const auto& [id, element] : m_Elements | std::views::reverse)
	{
		assert(element);

		// Serialize Arrow
		if (auto* arrow = dynamic_cast<Elements::ArrowElement*>(element.get()))
			SerializeArrow(root, *arrow);

		// Serialize Shape
		else if (auto* shape = dynamic_cast<Elements::ShapeElement*>(element.get()))
			SerializeShape(root, *shape);

		// Serialize Text
		else if (auto* text = dynamic_cast<Elements::TextElement*>(element.get()))
			SerializeText(root, *text);

		// Serialize Image
		else if (auto* image = dynamic_cast<Elements::ImageElement*>(element.get()))
			SerializeImage(root, *image);

		// Not implemented
		else
			LOG_WARN("Serialization for element type: {} is not implemented", typeid(*element).name());
	}
}

class PathData2
{
public:
	std::string AsString() const
	{
		std::string result;
		for (const auto& command : m_Commands)
		{
			result += command;
			result += " ";
		}
		return result;
	}

	PathData2& MoveTo(glm::vec2 point)
	{
		m_Commands.push_front(fmt::format("M {} {}", point.x, point.y));
		return *this;
	}

	PathData2& LineTo(glm::vec2 point)
	{
		m_Commands.push_back(fmt::format("L {} {}", point.x, point.y));
		return *this;
	}

	PathData2& QuadraticBezierTo(glm::vec2 control, glm::vec2 end)
	{
		m_Commands.push_back(fmt::format("Q {} {} {} {}", control.x, control.y, end.x, end.y));
		return *this;
	}
private:
	std::list<std::string> m_Commands;
};

void SvgSerializer2::SerializeArrow(tinyxml2::XMLElement& root, const Elements::ArrowElement& arrow)
{
	const auto& data = arrow.GetData();
	auto arrowBegin = data.Points.front().Position;

	auto pathData = PathData2 {}.MoveTo(arrowBegin);
	for (auto it = std::next(data.Points.begin()); it != data.Points.end(); it++)
	{
		pathData.LineTo(it->Position);
		// pathData.QuadraticBezierTo(it->Position, (++it)->Position);
	}

	auto path = root.InsertNewChildElement("path");
	path->SetAttribute("d", pathData.AsString().c_str());
	path->SetAttribute("stroke", GetColorString(data.StrokeColor).c_str());
	path->SetAttribute("stroke-width", data.Thickness);
	path->SetAttribute("fill", "none");
	path->SetAttribute("marker-end", "url(#arrowhead)");
}

void SvgSerializer2::SerializeShape(tinyxml2::XMLElement& root, const Elements::ShapeElement& shape)
{
	const auto& data = shape.GetData();
	auto& points = data.Points;

	// Serialize points as polygon
	// TODO: Check if the order of points is correct
	auto polygon = root.InsertNewChildElement("polygon");
	std::string pointsAsString;
	for (const auto& point : points)
	{
		pointsAsString += fmt::format("{},{} ", point.Position.x, point.Position.y);
	}
	polygon->SetAttribute("points", pointsAsString.c_str());
	polygon->SetAttribute("fill", GetColorString(data.Color).c_str());

	if (data.BlendMode == Render::BlendMode::Multiply)
	{
		polygon->SetAttribute("style", "mix-blend-mode: multiply;");
	}
}

void SvgSerializer2::SerializeText(tinyxml2::XMLElement& root, const Elements::TextElement& text)
{
	const auto& data = text.GetData();

	auto textElement = root.InsertNewChildElement("text");
	textElement->SetAttribute("x", data.Position.x);
	textElement->SetAttribute("y", data.Position.y);
	textElement->SetAttribute("text-anchor", "start");
	textElement->SetAttribute("dominant-baseline", "auto");
	textElement->SetAttribute("font-family", "Roboto");
	textElement->SetAttribute("font-size", data.FontSize);
	textElement->SetAttribute("letter-spacing", data.LetterSpacing);
	textElement->SetAttribute("fill", GetColorString(data.FontColor).c_str());

	SerializeTextContent(*textElement, text);
}

void SvgSerializer2::SerializeTextContent(tinyxml2::XMLElement& textElement, const Elements::TextElement& text)
{
	const auto& data = text.GetData();
	const bool keepNewlines = true;
	const auto contentInUtf8 = data.GetTextInUtf8();
	auto lines = Utils::Strings::SplitToLines(contentInUtf8, keepNewlines);
	float cursorX = textElement.FloatAttribute("x");
	float cursorY = textElement.FloatAttribute("y");
	int baselineHeight = Renderer::GetBaselineHeight(data.FontSize, data.FontId);
	for (const auto& line : lines)
	{
		auto tspan = textElement.InsertNewChildElement("tspan");
		tspan->SetText(std::string(line.data(), line.size()).c_str());
		tspan->SetAttribute("x", cursorX);
		tspan->SetAttribute("y", cursorY);
		cursorY += baselineHeight;
	}
}

static void SavePngData(void* context, void* data, int size)
{
	auto* result = (std::vector<uint8_t>*)context;
	result->clear();
	result->insert(result->end(), (uint8_t*)data, (uint8_t*)data + size);
}

static std::string SerializeTextureAsUriScheme(const Elements::ImageElement& image)
{
	const auto& data = image.GetData();
	auto rgba = Renderer::LoadBytesFromImage(*data.TextureId, data.Width, data.Height);
	const int channels = 4;
	std::vector<uint8_t> pngData;
	int success = stbi_write_png_to_func(
		SavePngData, &pngData, data.Width, 
		data.Height, channels, rgba.data(), 
		data.Width * channels);
	if (not success)
	{
		LOG_ERROR("Failed to serialize image as PNG");
		return "";
	}

	// Encode to base64
	int pngBase64Size = 0;
	char* pngBase64Data = EncodeDataBase64(pngData.data(), (int)pngData.size(), &pngBase64Size);
	std::string pngBase64String { pngBase64Data, (unsigned int)pngBase64Size };
	MemFree(pngBase64Data);

	return "data:image/png;base64," + pngBase64String;
}

void SvgSerializer2::SerializeImage(tinyxml2::XMLElement& root, const Elements::ImageElement& image)
{
	const auto& data = image.GetData();

	auto imageElement = root.InsertNewChildElement("image");
	imageElement->SetAttribute("x", data.Position.x);
	imageElement->SetAttribute("y", data.Position.y);
	imageElement->SetAttribute("width", data.Width);
	imageElement->SetAttribute("height", data.Height);
	imageElement->SetAttribute("href", SerializeTextureAsUriScheme(image).c_str());
}