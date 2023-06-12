#include "pch.hpp"
#include "SvgSerializer.hpp"
#include "Canvas/Components.hpp"
#include "Canvas/Entity.hpp"
#include "Render/Renderer.hpp"
#include <raylib.h>
#include <stb_image_write.h>
#include <tinyxml2.h>
#include <sstream>


namespace
{
	std::string GetColorString(glm::vec4 color)
	{
		std::stringstream ss;
		ss << "rgb(" << (int)(color.r * 255) << "," << (int)(color.g * 255) << "," << (int)(color.b * 255) << ")";
		return ss.str();
	}
}

SvgSerializer::SvgSerializer(const entt::registry& registry)
	: m_Registry(registry) {}

std::string SvgSerializer::Serialize()
{
	auto viewBox = GetCanvasViewBox();

	tinyxml2::XMLDocument doc;
	GenerateSvgRootElement(doc, viewBox);
	auto& root = *doc.RootElement();

	AddArrowheadMarkers(root);
	SerializeAllEntities(root);
	AddBorderToSvgDocument(root, viewBox);

	doc.SaveFile("test.svg");

	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);
	return printer.CStr();
}

void SvgSerializer::GenerateSvgRootElement(tinyxml2::XMLDocument& doc, ViewBox viewBox)
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

void SvgSerializer::AddBorderToSvgDocument(tinyxml2::XMLElement& root, ViewBox viewBox)
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

void SvgSerializer::AddArrowheadMarkers(tinyxml2::XMLElement& root)
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

ViewBox SvgSerializer::GetCanvasViewBox()
{
	auto entitiesWithTransform = m_Registry.view<Components::Transform>();

	ViewBoxBuilder viewBoxBuilder{};
	for (Entity entity : entitiesWithTransform)
	{
		auto point = entity.GetComponent<Components::Transform>().GetGlobalTranslation();
		viewBoxBuilder.AddPoint(point);
		UpdateCanvasViewBoxFromFocusArea(viewBoxBuilder, entity);
	}
	viewBoxBuilder.AddPadding(PADDING_SIZE);

	return viewBoxBuilder.Build();
}

void SvgSerializer::UpdateCanvasViewBoxFromFocusArea(ViewBoxBuilder& viewBoxBuilder, const Entity entity)
{
	if (entity.HasComponent<Components::Focusable>())
	{
		auto& transform = entity.GetComponent<Components::Transform>();
		auto& focusable = entity.GetComponent<Components::Focusable>();
		auto box = focusable.AsBox(transform);
		glm::vec2 focusPointMin = { box.x, box.y };
		glm::vec2 focusPointMax = { box.x + box.width, box.y + box.height };
		viewBoxBuilder.AddPoint(focusPointMin);
		viewBoxBuilder.AddPoint(focusPointMax);
	}
}

void SvgSerializer::SerializeAllEntities(tinyxml2::XMLElement& root)
{
    // Draw entities in order of their z-Index (assume that components are already sorted)
    for (Entity entity : m_Registry.view<Components::Transform>())
    {
        // Serialize Arrow
        if (entity.HasComponent<Components::Arrow>())
            SerializeArrow(root, entity);

        // Serialize Text
        if (entity.HasComponent<Components::Text>())
            SerializeText(root, entity);

        // Serialize Image
        if (entity.HasComponent<Components::Image>())
            SerializeImage(root, entity);
    }
}

class PathData
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

	PathData& MoveTo(glm::vec2 point)
	{
        m_Commands.push_front(fmt::format("M {} {}", point.x, point.y));
		return *this;
    }
	
	PathData& LineTo(glm::vec2 point)
	{
		m_Commands.push_back(fmt::format("L {} {}", point.x, point.y));
		return *this;
	}

	PathData& QuadraticBezierTo(glm::vec2 control, glm::vec2 end)
	{
		m_Commands.push_back(fmt::format("Q {} {} {} {}", control.x, control.y, end.x, end.y));
		return *this;
	}
private:
	std::list<std::string> m_Commands;
};

void SvgSerializer::SerializeArrow(tinyxml2::XMLElement& root, const Entity entity)
{
	auto& transform = entity.GetComponent<Components::Transform>();
	auto& arrow = entity.GetComponent<Components::Arrow>();
	auto arrowBegin = arrow.GetBegin(transform);
	auto arrowEnd = arrow.GetEnd(transform);
	auto controlPoint = arrow.ControlPoint ? std::make_optional(arrow.GetControlPoint(transform)) : std::nullopt;

	auto path = root.InsertNewChildElement("path");

	auto pathData = PathData{}.MoveTo(arrowBegin);
	if (controlPoint)
		pathData.QuadraticBezierTo(controlPoint.value(), arrowEnd);
	else
		pathData.LineTo(arrowEnd);

	path->SetAttribute("d", pathData.AsString().c_str());
	path->SetAttribute("stroke", GetColorString(arrow.StrokeColor).c_str());
	path->SetAttribute("stroke-width", arrow.Thickness);
	path->SetAttribute("fill", "none");
	path->SetAttribute("marker-end", "url(#arrowhead)");
}

void SvgSerializer::SerializeText(tinyxml2::XMLElement& root, const Entity entity)
{
	auto& transform = entity.GetComponent<Components::Transform>();
	auto& text = entity.GetComponent<Components::Text>();

	auto textElement = root.InsertNewChildElement("text");
	textElement->SetAttribute("x", transform.GetGlobalTranslation().x);
	textElement->SetAttribute("y", transform.GetGlobalTranslation().y);
	textElement->SetAttribute("text-anchor", "start");
	textElement->SetAttribute("dominant-baseline", "hanging");
	textElement->SetAttribute("font-family", "Arial"); // FIXME: font should not be hardcoded
	textElement->SetAttribute("font-size", text.FontSize);
	textElement->SetAttribute("letter-spacing", text.LetterSpacing);
	textElement->SetAttribute("fill", GetColorString(text.FontColor).c_str());
	textElement->SetText(text.Content.c_str());
}

static void SavePngData(void *context, void *data, int size)
{
    auto* result = (std::vector<uint8_t>*)context;
    result->clear();
    result->insert(result->end(), (uint8_t*)data, (uint8_t*)data + size);
}

std::string SerializeTextureAsUriScheme(Components::Image image)
{
    // Get image raw RGBA bytes and transform it to png encoded data
    auto data = Renderer::LoadBytesFromImage(image);
    const int channels = 4;
    std::vector<uint8_t> pngData;
    int success = stbi_write_png_to_func(SavePngData, &pngData, image.Width, image.Height, channels, data.data(), image.Width * channels);
    if (not success)
    {
        LOG_ERROR("Failed to serialize image as png");
        return "";
    }

    // Encode to base64
    int pngBase64Size = 0;
    char* pngBase64Data = EncodeDataBase64(pngData.data(), (int)pngData.size(), &pngBase64Size);
    std::string pngBase64String{pngBase64Data, (unsigned int)pngBase64Size};
    MemFree(pngBase64Data);

    return "data:image/png;base64," + pngBase64String;
}

void SvgSerializer::SerializeImage(tinyxml2::XMLElement& root, const Entity entity)
{
    auto& transform = entity.GetComponent<Components::Transform>();
    auto& image = entity.GetComponent<Components::Image>();

    auto imageElement = root.InsertNewChildElement("image");
    imageElement->SetAttribute("x", transform.GetGlobalTranslation().x);
    imageElement->SetAttribute("y", transform.GetGlobalTranslation().y);
    imageElement->SetAttribute("width", image.Width);
    imageElement->SetAttribute("height", image.Height);
    imageElement->SetAttribute("href", SerializeTextureAsUriScheme(image).c_str());
}